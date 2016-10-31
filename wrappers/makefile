SRCS = $(wildcard *.c)
SLIBS = $(patsubst %.c,%.so,$(SRCS))

all: $(SLIBS)

%.so: %.c rng.h
	gcc -fPIC -shared -o $@  $< -ldl

clean:
	rm -f *.so
