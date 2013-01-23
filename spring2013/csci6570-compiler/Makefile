# compiler to use
CC      = /usr/bin/gcc
CFLAGS  = -std=c99

# object files needed for the project
OBJS    = lex.yy.o tjc.o

test: tjc
	./tjc sample.java

# the main target
tjc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -ll

# dependencies
tjc.o: tjc.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

clean:
	$(RM) $(OBJS) lex.yy.c tjc
