# should be invoked as e.g.
#	make PROG="malloc" test_utils
# there should be a file in the directory called PROG_wrapper.c, e.g. malloc_wrapper.c

RUN = ./wrapped_run.sh
FILES = files
test_utils:
	./compile.sh ${PROG}_wrapper
	$(RUN) ${PROG} grep $(FILES)/foo.txt -e bar
	$(RUN) ${PROG} cat $(FILES)/foo.txt
	$(RUN) ${PROG} gcc $(FILES)/foo.c -o /dev/null
	$(RUN) ${PROG} zip $(FILES)/foo $(FILES)/foo.txt
	$(RUN) ${PROG} unzip -l $(FILES)/foo.zip

