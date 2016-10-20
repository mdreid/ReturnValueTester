import sys
from pycparser import c_parser, c_ast, c_generator

text = r"""
int open(const char *pathname, int flags);
"""

parser = c_parser.CParser()
ast = parser.parse(text, filename='<none>')

decl_node = ast.ext[-1]
print("name: %s" % decl_node.name)

decl = decl_node.type
gen = c_generator.CGenerator()
print("whole sig: %s" % gen.visit(decl))
print("return: %s" % gen.visit(decl.type))
for param in decl.args.params:
    print("param: %s" % gen.visit(param))


with open('open_wrapper.c', 'w') as f:
    f.write('#define _GNU_SOURCE\n#include <dlfcn.h>\n#include <stdio.h>\n\n')
    f.write('static %s (*real_open) (%s) = NULL;\n\n' % (gen.visit(decl.type), ', '.join([gen.visit(param) for param in decl.args.params])))
    f.write('%s {\n' % gen.visit(decl))     # int open(const char *pathname, int flags) {
    f.write('  printf("wrapped open\\n");\n')  # 
    f.write('  real_open = dlsym(RTLD_NEXT, "open");\n')
    f.write('  return real_open(%s);\n' % ', '.join([param.name for param in decl.args.params]))
    f.write('}\n')



