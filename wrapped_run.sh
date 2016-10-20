#!/bin/bash
#usage: . wrapped_run.sh <name_of_call> <program + args>
#ex: . wrapped_run.sh read grep foo.txt -e bar

LD_PRELOAD=$PWD/$1_wrapper.so ${@:2}
