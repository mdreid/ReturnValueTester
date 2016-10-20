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
