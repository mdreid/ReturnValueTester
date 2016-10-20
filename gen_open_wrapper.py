import sys
from pycparser import c_parser, c_ast, c_generator

# example method signature
text = "int open(const char *pathname, int flags);"

# parse the AST
parser = c_parser.CParser()
ast = parser.parse(text, filename='<none>')

decl_node = ast.ext[-1]
decl = decl_node.type
gen = c_generator.CGenerator()

with open('open_wrapper.c', 'w') as f:
    f.write('#define _GNU_SOURCE\n')
    f.write('#include <dlfcn.h>\n')
    f.write('#include <stdio.h>\n\n')
    f.write('static %s (*real_open) (%s) = NULL;\n\n' % (gen.visit(decl.type), ', '.join([gen.visit(param) for param in decl.args.params])))
    f.write('%s {\n' % gen.visit(decl))     # int open(const char *pathname, int flags) {
    f.write('  printf("wrapped open\\n");\n')  # 
    f.write('  real_open = dlsym(RTLD_NEXT, "open");\n')
    f.write('  return real_open(%s);\n' % ', '.join([param.name for param in decl.args.params]))
    f.write('}\n')



