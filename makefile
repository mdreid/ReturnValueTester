WRAPPERS = $(wildcard wrappers/*wrapper.c)
SLIBS = $(patsubst wrappers/%.c,wrappers/%.so,$(WRAPPERS))

all: $(SLIBS) 

wrappers/%.so: wrappers/%.c wrappers/rng.h
	gcc -fPIC -shared -o $@  $< -ldl -g

clean:
	rm -f wrappers/*.so



# should be invoked as e.g.
#	make PROG=malloc test_utils
#	make PROG=malloc compile
# there should be a file in the directory called PROG_wrapper.c, e.g. malloc_wrapper.c

#RUN = ./wrapped_run.sh
#FILES = files

#test_utils: compile
#	$(RUN) ${PROG} grep $(FILES)/foo.txt -e bar
#	$(RUN) ${PROG} cat $(FILES)/foo.txt
#	$(RUN) ${PROG} gcc $(FILES)/foo.c -o /dev/null
#	$(RUN) ${PROG} zip $(FILES)/foo $(FILES)/foo.txt
#	$(RUN) ${PROG} unzip -l $(FILES)/foo.zip

