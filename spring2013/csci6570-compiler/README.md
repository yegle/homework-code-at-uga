# TinyJava compiler project

by Yuchen Ying <yegle@uga.edu>

## Lexer

### Build

run `make` in the source code root directory

### Run

I wrote two different lexer, one in C, one in Python.

For the C version: run `./tjc XXX.java` at the source code root
directory.

For the Python version: run `python pylexer.py` at the source code root
directory.

## Parser

### Build

Run `make parser` or simply run `make` in the source code root directory

### Run

Run `./parser XXX.java` at the source code root directory.


## Final compiler

### Build

Run make

### Run

Run `tjc xxx.java > xxx.jsm` then compile with `java -jar jasmine.jar
xxx.jsm`

