import sys
import os
from pycparser import c_generator, preprocess_file, c_ast
from pycparserext.ext_c_parser import GnuCParser
from pycparserext.ext_c_generator import GnuCGenerator

# header_file: path to header file to read and process
# out_dir: directory to write files to
# fn_names: "*", to indicate that all fn decls in file should have wrappers generated, or a
#    list of triplets, where each contains:
#      - function name, 
#      - errno, and 
#      - return value
# just_list: only list functions declared in this header, don't write wrappers to file
def write_files(dir_list, header_include, out_dir, fn_names):
    header_file = header_include.split('/')[-1]

    is_valid_file = [os.path.isfile(dir_name + header_file) for dir_name in dir_list]
    
    if not any(is_valid_file):
        print "Skipped all functions in %s because it could not be found in %s." % (header_file, dir_list)
        return
    else:
        header_file = dir_list[is_valid_file.index(True)] + header_file

    # pycparser utility function for preprocessing the file and
    # getting all of the file as a string
    text = preprocess_file(header_file)

    # use pycparserext to parse the GNU C header file
    p = GnuCParser()
    ast = p.parse(text)

    # loop over all functions requested
    for fn in fn_names:
        found = False
        
        # loop over the ast
        for node in ast.ext:
            if isinstance(node, c_ast.FuncDef) or not isinstance(node.type, c_ast.FuncDecl):
                continue

            # skip over functions with __noreturn__
            if hasattr(node.type.type, 'attributes') and any([e.name == "__noreturn__" for e in node.type.type.attributes.exprs]):
                continue

            if node.name != fn[0]:
                continue
            
            found = True

            # check for variadic args, skip if found
            if any([isinstance(arg, c_ast.EllipsisParam) for arg in node.type.args.params]):
                print "Skipped function %s because of variadic args." % fn[0]
                continue

            write_file(out_dir, header_include, node, fn[1], fn[2])

        if not found:
            print "The header file %s does not contain a method signature for %s" % (header_file, fn[0])

# out_dir: directory to write out to
# header_file: name of header file to #include at the top
# node: ast node with fn decl
# errno: value to return for errno, as a string
# retval: what to return, as a string
def write_file(out_dir, header_include, node, errno, retval):
    header = """
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <%s>
"""

    code_template = """
static %s %s(*%s) (%s) = NULL;
%s {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  %s = dlsym(RTLD_NEXT, "%s");
  if(flip || (%s == NULL)) {
    %s
    return %s;
  } else {
    %s
  }
}
"""
    
    gen = GnuCGenerator()

    new_fn = node.name
    real_fn = "real_" + new_fn

    arglist = gen.visit(node.type.args)

    is_ptr_result = isinstance(node.type.type, c_ast.PtrDecl)
    is_void_result = gen.visit(node.type.type) == "void" and not is_ptr_result

    # eliminate __extension__ and stuff like it from the function signature
    func_sig = "%s %s %s%s(%s)" % (
        ' '.join(node.storage), 
        gen.visit(node.type.type), 
        '*' if is_ptr_result else '',
        new_fn, 
        arglist
    )

    new_file = out_dir + '%s_wrapper.c' % new_fn
    with open(new_file, 'w') as f:
        f.write(header % header_include)
        f.write( code_template % (
            gen.visit(node.type.type), # int
            '*' if isinstance(node.type.type, c_ast.PtrDecl) else '',            
            real_fn,                   # real_open
            gen.visit(node.type.args), # (args with types)
            func_sig,
            real_fn,                   # real_open
            new_fn,
            real_fn,                   # real_open
            ("errno = %s;" % errno) if errno is not None else "",
            retval,
            "%s%s(%s);" % (            # return
                "return " if not is_void_result else "",
                real_fn,               # real_open
                ', '.join([a.name if a.name is not None else '' for a in node.type.args.params]),
            )
        ))
    print 'wrote %s' % new_file

def get_wrapper_info(filename):
    IDX_NAME = 0
    IDX_RET = 1
    IDX_ERR = 2
    IDX_HEADER = 3

    fns = {}
    with open(filename, 'r') as f:
        lines = lis=[line.split(',') for line in f]

    header_files = set(line[IDX_HEADER] for line in lines[1:])
    for f in header_files:
        fns[f] = []
        
    for line in lines[1:]:
        fns[line[IDX_HEADER]].append((line[IDX_NAME], line[IDX_ERR] if line[IDX_ERR] != '' else None, line[IDX_RET]))

    return fns

if __name__ == "__main__":
    usage = "Usage: python gen_wrappers.py <input_file> <out_dir>"
    assert len(sys.argv) == 3, usage

    wrappers_to_gen = get_wrapper_info(sys.argv[1])

    for header_file in wrappers_to_gen:
        write_files(['/usr/include/','/usr/include/x86_64-linux-gnu/sys/', '/usr/include/linux/'], header_file, sys.argv[2], wrappers_to_gen[header_file])
