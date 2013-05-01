# compiler to use
CC = /usr/bin/g++
CXX = /usr/bin/g++
CFLAGS = -fPIC -I./ast
CXXFLAGS = -fPIC -I./ast
YFLAGS = -yd

LEXFILE = TinyJava.l
YACCFILE = TinyJava.y

SRCS	:= $(wildcard *.cpp)
OBJS	:= $(SRCS:.cpp=.o) tjc_bison.o tjc_flex.o

TEMPFILES := tjc_flex.c tjc_bison.c tjc y.tab.h $(wildcard *.o)

LEX = /usr/bin/flex
YACC = /usr/bin/bison

LDFLAGS = -L./ast
LDLIBS = -lAst -lfl

all: tjc_bison.o tjc_flex.o libtjlex.so tjc libtjparse.so y.tab.h y.tab.c tjc

tjc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

test: tjc
	./test.sh

#y.tab.c y.tab.h: $(YACCFILE)
#	$(YACC) $(YFLAGS) $<

y.tab.c: tjc_flex.c

libtjlex.so: tjc_flex.o
	$(CXX) $(CXXFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

libtjparse.so: tjc_bison.o
	$(CXX) $(CXXFLAGS) -lfl -ll -shared -Wl,-soname,$@ -o $@ $<

#klex.yy.c: TinyJava.l
#k	$(LEX) $<

clean:
	$(RM) $(OBJS) $(TEMPFILES)

SymbolTableBuilder.o SymbolTable.o: 
