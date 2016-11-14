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