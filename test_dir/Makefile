all: blaster blastee
blaster: blaster.c shared.o shared.h
	gcc -o blaster blaster.c shared.o
shared.o: shared.c shared.h
	gcc -c shared.c
blastee: blastee.c shared.o shared.h
	gcc -o blastee blastee.c shared.o
clean:
	rm blaster blastee shared.o
