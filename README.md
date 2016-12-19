# ReturnValueTester

Keith Funkhouser (wfunkhouser@cs.wisc.edu)

Malcolm Reid (mreid3@wisc.edu)

Colin Samplawski (csamplawski@wisc.edu)

This repo contains the code that was used to conduct our final research project in CS736.
Here is a quick run down of the files within and what they do:

**Warning**: Many of these scripts require an environment variable called PROB to be set to a given percent (.1 for 10%, .2 for 20%, etc). This percent is how often an error will be returned to the calling program. PROB can be set using the command:  `export PROB=.1` 

* misc/ - A collection of old files. Not in use, but seemed like they were worth keeping.
* paper/ - the tex source of our paper
* proposal/ - the tex source of our proposal
* test_dir - a number of files needed to run our automatic testing of core utils
  * test_dir/full_utils.txt - the complete list of utils tested and the associated args  
    The synatx of the file is as follows: `<util>|<PRE>|<AFTER>|<args>`. Here PRE and AFTER are commands that must be executed before or after the util is called. For example, a file must exist in order for cat to read it.
  * test_dir/test.sh - generates a log for each call in wrappers/ using a file in the same form as full_utils.txt   
    Runs the program 20 times for each probability p in 0%,10%,...,50%
* wrapper_gen/ - code we used to automatically generate our wrappers  
  Note that these files depend on non-standard Python modules (see README)
* wrappers/ - our auto-generated wrappers
* count_calls.py - the script we used to count the systems/library calls that utils make 
* log_gen - script that generates log files for a given util
  * usage: `log_gen <num_iters> <call to wrap> <program + args>`  
  example: `log_gen 20 read ls -la`  
    Note that read_wrapper.so must exist in wrappers for this to work. Similary PROB must be set.
* makefile - generates .so files from our wrappers
* seeded_run - runs a program with a particular seed and call
  * usage: `. seeded_run.sh <seed> <call to wrap> <program + args>`
* unseeded_run.sh - runs a program for a particular call and generates a seed
  * usage: `. unseeded_run.sh <call to wrapper> <program + args>`
  
  


