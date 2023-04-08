# Operating-Systems

Project for university assignment.

## Build

Simply run `make`, you can set your compiler or include extra compiler flags by
editing the Makefile.

You can redirect the process `stderr` to `/dev/null` in case you want to only
see the display without debug logs.

```
$ make 2> /dev/null
```

## Dependencies

- `phtread` support
- A C++17 compiler (`clang++` or `g++`)

