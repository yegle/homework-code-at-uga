# compiler to use
CC      = gcc
CFLAGS  = -std=c99 -fPIC

# object files needed for the project
OBJS    = lex.yy.o tjc.o

all: tjc libtjlex.so parser

parser: parser.c lex.yy.c y.tab.c
	$(CC) $^ -o $@

# the main target
tjc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lfl

y.tab.c: TinyJava.y
	yacc --report=all $<

y.tab.h: TinyJava.y
	yacc -d --debug $<

# dependencies
tjc.o: tjc.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c -o $@ $<

libtjlex.so: lex.yy.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

clean:
	$(RM) $(OBJS) lex.yy.c tjc libtjlex.so y.tab.c
