parse:
	python2.7 test_parsing_header_file.py stdlib.h >| stdlib.c
compile:
	gcc -fPIC -shared -o stdlib.so stdlib.c -ldl
clean:
	rm -f *.c *.so
