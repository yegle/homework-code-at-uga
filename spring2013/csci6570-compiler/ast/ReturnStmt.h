// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __RETURNSTMT_H__
#define __RETURNSTMT_H__

#include <stdio.h>

#include "Statement.h"
#include "Expression.h"
#include "AstVisitor.h"


class ENTRY;




/**
 * @brief This class represents a TinyJava return statement, with or without an expression.
 * 
 * A return statement is represented by:
 * - #an Expression representing the value to be returned (may be NULL)
 * - #a pointer to the symbol table ENTRY for the name may also be included (may be initially NULL).
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class ReturnStatement: public Statement {

private:
  ENTRY      *entry;		// enclosing method's symtab entry
  Expression *expr;		// the return expression; may be NULL
  
public:

  /** 
   * Create a new ReturnStatement object.
   * 
   * @param lno is a source line number with the return statement
   * @param e is a pointer to the symbol tabel ENTRY representing the method
   * @param exp is a pointer to an expression node representing the return value expression
   * @throws AstException if the symbol table ENTRY or the expression is NULL
   */
  ReturnStatement( int lno, ENTRY *e, Expression *exp )
  {
    setKind( SRETURN );
    setLineNo( lno );
    entry    = e;
    expr     = NULL;
    if( exp != NULL )
      expr = exp;
  }

  /**
   * Return the symbol table ENTRY representing the method within which the return statement is included.
   *
   * @return ENTRY pointer to the symbol table ENTRY representing the method of the return
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the symbol table ENTRY representing the method within which the return statement is included.
   * 
   * @param e a pointer to a new symbol table ENTRY representing the method of the return
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /**
   * Return the Expression pointer representing the return expression.
   *
   * @return Expression pointer representing the return value expression.
   */
  Expression *getExpression()
  {
    return expr;
  }

  /**
   * Accept a visitor to this node.
   * @param aVisitor is a visitor object of type AstVisitor.
   */
  void accept( AstVisitor *aVisitor )
  {
    aVisitor->visit( this );
  }

};




#endif
