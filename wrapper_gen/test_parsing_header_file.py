import sys

sys.path.append("pycparser-master")
from pycparser import c_generator, preprocess_file

# this is a fork of pycparser that can parse GNU extensions
sys.path.append("pycparserext")
from pycparserext.ext_c_parser import GnuCParser

def main(filename):
    # pycparser utility function for preprocessing the file and
    # getting all of the file as a string
    text = preprocess_file(filename)
    
    # use pycparserext to parse the GNU C header file
    p = GnuCParser()
    ast = p.parse(text)

    ast.show()

if __name__ == "__main__":
    assert len(sys.argv) == 2, "Must include filename argument for the header file!"
    main(sys.argv[1])
