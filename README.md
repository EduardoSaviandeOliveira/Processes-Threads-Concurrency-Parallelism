# Operating-Systems

Project for university assignment.

## Build (all)

Simply run `make`, you can set your compiler or include extra compiler flags by
editing the Makefile.

You can redirect the process `stderr` to `/dev/null` in case you want to only
see the display without debug logs.

```
$ make 2> /dev/null
```

## Threaded Solution



## FIFO Solution

This directory uses named pipes for IPC.
Example (run each command in separate terminals)

The `display` binary must be run first to establish the named pipes, the belt
processes may be run in any order.

```
$ ./display 1000
```
```
$ ./belt 1000 2 belt0.fifo
```
```
$ ./belt 2000 5 belt1.fifo
```


## Dependencies

- `phtread` support
- A C++17 compiler (`clang++` or `g++`)

