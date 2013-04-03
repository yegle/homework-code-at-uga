// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __ASTPRINTER_H__
#define __ASTPRINTER_H__

#include <iostream>
using namespace std;


#include "Ast.h"


class AstPrinter: public AstVisitor {

private:
  int   indent;

  // indent for nested printing
  void doIndent( int lev )
  {
    // indent
    for( int i = 0; i < lev; i++ )
      cout << "  ";
  }

public:

  AstPrinter( int indentLevel )
  {
    indent = indentLevel;
  }

  void visit( MethodDeclaration *aDeclNode );
  void visit( FieldDeclaration *aDeclNode );
  void visit( ParameterDeclaration *aDeclNode );
  void visit( VariableDeclaration *aDeclNode );
  void visit( ClassDeclaration *aDeclNode );
  void visit( LiteralExpression *anExpNode );
  void visit( ReferenceExpression *anExpNode );
  void visit( NewExpression *anExpNode );
  void visit( UnaryExpression *anExpNode );
  void visit( BinaryExpression *anExpNode );
  void visit( CastExpression *anExpNode );
  void visit( MethodCallExpression *anExpNode );
  void visit( AssignStatement *anStmtNode );
  void visit( WhileStatement *anStmtNode );
  void visit( ForStatement *anStmtNode );
  void visit( IfStatement *anStmtNode );
  void visit( ReturnStatement *anStmtNode );
  void visit( BlockStatement *anStmtNode );
  void visit( MethodCallStatement *anStmtNode );
  void visit( EmptyStatement *anStmtNode );
};



#endif
