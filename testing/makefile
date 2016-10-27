exec:
	gcc test_exec.c -o test_exec.o
	gcc dummy_executable.c -o dummy_executable

test_exec: exec
	../wrapped_run.sh ../open ./test_exec.o

fork:
	gcc test_fork.c -o test_fork.o

test_fork: fork
	../wrapped_run.sh ../open ./test_fork.o

static:
	gcc -static test_static.c -o test_static.o

test_static: static
	../wrapped_run.sh ../open test_static.o

call:
	gcc test_function_call.c test_function_call2.c -o test_function_call.o

test_call: call
	../wrapped_run.sh ../open test_function_call.o

clean:
	rm -f *.o
