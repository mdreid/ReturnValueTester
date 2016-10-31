import sys

sys.path.append("pycparser-master")
from pycparser import c_generator, preprocess_file, c_ast

# this is a fork of pycparser that can parse GNU extensions
sys.path.append("pycparserext")
from pycparserext.ext_c_parser import GnuCParser
from pycparserext.ext_c_generator import GnuCGenerator

def main(filename):
    # pycparser utility function for preprocessing the file and
    # getting all of the file as a string
    text = preprocess_file(filename)
    
    # use pycparserext to parse the GNU C header file
    p = GnuCParser()
    ast = p.parse(text)
    gen = GnuCGenerator()

    header = """
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <%s>
    """

    code_template = \
    """
static %s %s(*%s) (%s) = NULL;
%s {
  printf("%s");
  %s = dlsym(RTLD_NEXT, "%s");
  %s
}
    """
    
    for node in ast.ext:
        if isinstance(node, c_ast.FuncDef) or not isinstance(node.type, c_ast.FuncDecl):
            continue

        # skip over functions with __noreturn__
        if hasattr(node.type.type, 'attributes') and any([e.name == "__noreturn__" for e in node.type.type.attributes.exprs]):
            print 'skipped %s because of __noreturn__' % node.name
            continue

        # check for variadic args, skip if found
        if any([isinstance(arg, c_ast.EllipsisParam) for arg in node.type.args.params]):
            print 'skipped %s because of variadic arg' % node.name
            continue

        new_fn = node.name
        real_fn = "real_" + new_fn

        arglist = gen.visit(node.type.args)

        # eliminate __extension__ and stuff like it from the function signature
        func_sig = "%s %s %s%s(%s)" % (
            ' '.join(node.storage), 
            gen.visit(node.type.type), 
            '*' if isinstance(node.type.type, c_ast.PtrDecl) else '',
            new_fn, 
            arglist
        )

        new_file = './c/%s_wrapper.c' % new_fn
        with open(new_file, 'w') as f:
            f.write( header % filename )
            f.write( code_template % (
                gen.visit(node.type.type), # int
                '*' if isinstance(node.type.type, c_ast.PtrDecl) else '',            
                real_fn,                   # real_open
                gen.visit(node.type.args), # (args with types)
                func_sig,
                #"%s %s(%s)" % (gen.visit(node.type.type), new_fn, gen.visit(node.type.args)),
                #gen.visit(node),          # int open( (args with types) )
                "called %s!\\n" % new_fn,
                real_fn,                   # real_open
                new_fn,                    # open
                "%s%s(%s);" % (       # return
                    "return " if gen.visit(node.type.type) != "void" else "",
                    real_fn,                   # real_open
                    ', '.join([a.name if a.name is not None else '' for a in node.type.args.params]),
                )
            ))
        print 'wrote %s' % new_file

if __name__ == "__main__":
    assert len(sys.argv) == 2, "Usage: python gen_wrappers.py <file.h>"
    main(sys.argv[1])
