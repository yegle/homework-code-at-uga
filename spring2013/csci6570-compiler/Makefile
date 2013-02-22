# compiler to use
CC      = gcc
CFLAGS  = -std=c99 -fPIC

# object files needed for the project
OBJS    = lex.yy.o check_filename.o 

all: tjc libtjlex.so tjc lexer libtjparse.so

tjc: $(OBJS) y.tab.o parser.o
	$(CC) $(CFLAGS) -o $@ $^ -lfl

parser.o: parser.c

test: tjc
	./test.sh

lex.yy.o: lex.yy.c

y.tab.o: y.tab.c y.tab.h

# the main target
lexer: $(OBJS) tjc.o
	$(CC) $(CFLAGS) -o $@ $^ -lfl

check_filename.o: check_filename.c

y.tab.c: TinyJava.y
	bison --report=all --report-file=y.output -b y $<

y.tab.h: TinyJava.y
	bison -d --debug -b y --report=all --report-file=y.output $<

# dependencies
tjc.o: tjc.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c -o $@ $<

libtjlex.so: lex.yy.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

libtjparse.so: y.tab.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

clean:
	$(RM) $(OBJS) lex.yy.c tjc parser *.o *.so y.tab.* y.output
