# ReturnValueTester
Advanced Operating Systems Project

Notes:
- - -
# Core dump notes
- Core dumps are generated by calls to `kill`, but we are unable to intercept `kill` calls because the program is `fork`ed before `LD_PRELOAD` is set
- We may be able to use Google's `coredumper` to dump core files while running
  - [Stack Overflow question](http://stackoverflow.com/a/979297)
  - [Google Code link](https://code.google.com/archive/p/google-coredumper/)
- We can use the `backtrace` and `backtrace_symbols` to dump info about the current state of the stack:
  - [GNU](https://www.gnu.org/software/libc/manual/html_node/Backtraces.html)
  - Every time we intercept a system call, we should dump out the most recent version of the stack
  - Then, between this and the core dump, we should be able to infer where the program crashed
- Working with core files:
  - [Stack Overflow](http://stackoverflow.com/a/8806534): `gdb /path/to/binary /path/to/core`
- To get core files working in VM:
  - Set rules for `ulimit`:
    - Add a line to `/etc/security/limits.conf`:
    - `*	-	core	unlimited`
  - Set `core_pattern` to point to a directory that you can write, and with a useful file pattern (see `core(5)`)
    - e.g. `sudo sysctl -w kernel.core_pattern=$HOME'/core/%e_%p_%i_%t.core'`
    - Make sure that the directory exists: `mkdir ~/core`
- Debugging coreutils with gdb:
  - Download and build [coreutils](http://stackoverflow.com/questions/22005048/compile-specific-source-file-in-linux-coreutils-package)
  - `gdb <util>`
  - `set env set environment LD_PRELOAD=./wrappers/malloc_wrapper.so`
  - `break /u/w/f/wfunkhouser/private/cs736/final/ReturnValueTester/wrappers/malloc_wrapper.c:malloc`
