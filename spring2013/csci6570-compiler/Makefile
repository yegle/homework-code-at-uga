# compiler to use
CC = /usr/bin/g++
CXX = /usr/bin/g++
CFLAGS = -fPIC -I./ast
CXXFLAGS = -fPIC -I./ast
YFLAGS = -yd

LEXFILE = TinyJava.l
YACCFILE = TinyJava.y

SRCS	:= $(wildcard *.cpp)
OBJS	:= $(SRCS:.cpp=.o) y.tab.o lex.yy.o

TEMPFILES := $(wildcard y.tab.*) $(wildcard lex.yy.*) $(wildcard rec*)

LEX = /usr/bin/flex
YACC = /usr/bin/bison

LDFLAGS = -L./ast
LDLIBS = -lAst -lfl

all: tjc libtjlex.so tjc libtjparse.so y.tab.h y.tab.c

tjc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

test: tjc
	./test.sh

y.tab.c y.tab.h: $(YACCFILE)
	$(YACC) $(YFLAGS) $<

libtjlex.so: lex.yy.o
	$(CXX) $(CXXFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

libtjparse.so: y.tab.o
	$(CXX) $(CXXFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

lex.yy.c: TinyJava.l
	$(LEX) $<

clean:
	$(RM) $(OBJS) $(TEMPFILES)

SymbolTableBuilder.o SymbolTable.o: y.tab.h
