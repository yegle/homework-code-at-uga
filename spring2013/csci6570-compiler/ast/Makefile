##
## Version:     1
##
## Date:        02/22/2012
##
## TinyJava Abstract Syntax Tree 
##
## Makefile
##
## K.J. Kochut
##


CXX		= g++
CPPFLAGS	=
CXXFLAGS	= -g 
LDFLAGS		= 
LDLIBS		=

SRCS            = AstNode.cpp AssignStmt.cpp BlockStmt.cpp ClassDeclaration.cpp IfStmt.cpp \
		  BinaryExpression.cpp CastExpression.cpp ForStmt.cpp MethodCallExpression.cpp \
		  MethodDeclaration.cpp

HDRS            = AstNode.h Declaration.h ClassDeclaration.h FieldDeclaration.h MethodDeclaration.h \
		  ParameterDeclaration.h VariableDeclaration.h \
		  Expression.h LiteralExpression.h ReferenceExpression.h BinaryExpression.h \
		  NewExpression.h UnaryExpression.h CastExpression.h MethodCallExpression.h \
	          Statement.h AssignStmt.h IfStmt.h BlockStmt.h EmptyStmt.h WhileStmt.h ReturnStmt.h \
		  ForStmt.h MethodCallStmt.h \
		  AstException.h AstVisitor.h 

OBJS	        = AstNode.o AssignStmt.o BlockStmt.o ClassDeclaration.o IfStmt.o \
		  BinaryExpression.o CastExpression.o ForStmt.o MethodCallExpression.o \
		  MethodDeclaration.o


# compile rules
#
COMPILE.cpp     = $(CXX) $(CPPFLAGS) $(CXXFLAGS)  -c
LINK.cpp        = $(CXX) $(CXXFLAGS) $(LDFLAGS)

.SUFFIXES: .cpp

.cpp.o:
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

all: libAst.a

libAst.a: Makefile $(OBJS)
	ar -cvq $@ $(OBJS)

clean:
	/bin/rm -f *.o

realclean: clean
	/bin/rm -f libAst.a

#################################################################
#################################################################
## Dependencies
##
AssignStmt.o: AssignStmt.cpp AssignStmt.h AstException.h AstVisitor.h \
 Expression.h AstNode.h Statement.h
AstNode.o: AstNode.cpp AstNode.h AstException.h
BinaryExpression.o: BinaryExpression.cpp BinaryExpression.h \
 AstException.h Expression.h AstNode.h AstVisitor.h
BlockStmt.o: BlockStmt.cpp BlockStmt.h AstException.h Statement.h \
 AstNode.h AstVisitor.h
CastExpression.o: CastExpression.cpp CastExpression.h AstException.h \
 AstVisitor.h Expression.h AstNode.h
ClassDeclaration.o: ClassDeclaration.cpp ClassDeclaration.h \
 AstException.h AstVisitor.h Declaration.h AstNode.h
ForStmt.o: ForStmt.cpp ForStmt.h AstException.h Statement.h AstNode.h \
 AstVisitor.h
IfStmt.o: IfStmt.cpp IfStmt.h AstException.h Expression.h AstNode.h \
 Statement.h AstVisitor.h
MethodCallExpression.o: MethodCallExpression.cpp MethodCallExpression.h \
 Expression.h AstNode.h AstVisitor.h AstException.h
MethodDeclaration.o: MethodDeclaration.cpp MethodDeclaration.h \
 Declaration.h AstNode.h Statement.h AstException.h AstVisitor.h

