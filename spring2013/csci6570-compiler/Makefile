# compiler to use
CC      = gcc

# object files needed for the project
OBJS    = lex.yy.o test.o

# the main target
test: $(OBJS)
	$(CC) -o $@ $(OBJS) -ll

# dependencies
test.o: test.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h
	$(CC) -c -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

clean:
	$(RM) $(OBJS) lex.yy.c test
