WRAPPERS = $(wildcard wrappers/*wrapper.c)
SLIBS = $(patsubst wrappers/%.c,wrappers/%.so,$(WRAPPERS))

all: $(SLIBS) 

wrappers/%.so: wrappers/%.c wrappers/rng.h
	gcc -fPIC -shared -o $@  $< wrappers/rng.c -ldl -g

clean:
	rm -f wrappers/*.so

combined:
	gcc -fPIC -shared -o wrappers/combined_wrapper.so wrappers/*_wrapper.c wrappers/rng.c -ldl -g
