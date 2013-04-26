# compiler to use
CC = /usr/bin/gcc
CXX = /usr/bin/g++
CFLAGS = -std=c99 -fPIC -I./ast
CXXFLAGS = -fPIC -I./ast

# object files needed for the project
OBJS    = lex.yy.o check_filename.o y.tab.o AstPrinter.o SymbolTable.o SymbolTableBuilder.o CodeGen.o parser.o

LEX = /usr/bin/flex
BISON = /usr/bin/bison

LDFLAGS = -L./ast
LDLIBS = -lAst -lfl

.PHONY: symboltable

all: tjc libtjlex.so tjc libtjparse.so symboltable

tjc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

parser.o: parser.c
	$(CXX) -c $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

test: tjc
	./test.sh

y.tab.o: y.tab.c y.tab.h ast/Ast.h
	$(CXX) -c $(CXXFLAGS) -o $@ y.tab.c $(LDFLAGS) $(LDLIBS)

check_filename.o: check_filename.c
	$(CXX) -c $(CXXFLAGS) -o $@ $^

y.tab.c: TinyJava.y
	$(BISON) --report=all --report-file=y.output -b y $<

y.tab.h: TinyJava.y
	$(BISON) -d --debug -b y --report=all --report-file=y.output $<

# dependencies
tjc.o: tjc.c y.tab.h
	$(CXX) -c $(CXXFLAGS) -o $@ tjc.c

lex.yy.o: lex.yy.c y.tab.h ast/Ast.h
	$(CXX) -c $(CXXFLAGS) -o $@ lex.yy.c

libtjlex.so: lex.yy.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

libtjparse.so: y.tab.o
	$(CC) $(CFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) TinyJava.l

AstPrinter.o: AstPrinter.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS) lex.yy.c tjc parser *.o *.so y.tab.* y.output

SymbolTable.o: SymbolTable.cpp SymbolTable.h
	$(CXX) $(CXXFLAGS) -g -c -o $@ $<

SymbolTableBuilder.o: SymbolTableBuilder.cpp SymbolTableBuilder.h
	$(CXX) $(CXXFLAGS) -g -c -o $@  $<

CodeGen.o: CodeGen.cpp CodeGen.h
	$(CXX) $(CXXFLAGS) -g -c -o $@ $<
