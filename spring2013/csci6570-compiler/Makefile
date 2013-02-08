# compiler to use
CC      = gcc
CFLAGS  = -std=c99 -fPIC

# object files needed for the project
OBJS    = lex.yy.o tjc.o

test: all
	./tjc sample.java

all: tjc libtjlex.so

# the main target
tjc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lfl

y.tab.h: TinyJava.y
	yacc -d $<

# dependencies
tjc.o: tjc.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c -o $@ $<

libtjlex.so: lex.yy.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

clean:
	$(RM) $(OBJS) lex.yy.c tjc libtjlex.so
