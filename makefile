WRAPPERS = $(wildcard wrappers/*wrapper.c)
SLIBS = $(patsubst wrappers/%.c,wrappers/%.so,$(WRAPPERS))

all: $(SLIBS) 

wrappers/%.so: wrappers/%.c wrappers/rng.h
	gcc -fPIC -shared -o $@  $< -ldl -g

clean:
	rm -f wrappers/*.so
