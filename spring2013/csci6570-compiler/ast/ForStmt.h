
// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __FORSTMT_H__
#define __FORSTMT_H__


#include "AstException.h"
#include "Statement.h"
#include "AstVisitor.h"

class ENTRY;



/**
 * @brief This class represents a TinyJava FOR statement.
 * 
 * The syntax of the for statement is either
 * - #FOR '(' IDENT '=' initExpression ';' termExpression ';' updateExpression ')' statement
 *
 * or
 *
 * - #FOR '(' IDENT '[' indexExpression ']' '=' initExpression ';' termExpression ';' updateExpression ')' statement
 *
 * A for statement is represented by:
 * - #identifier on the left-hand side of the initialization expression
 * - #an index Expression, if the left hand side is an array element
 * - #an initialization Expression
 * - #a termination Expression
 * - #an update Expression, and
 * - #a for statement body, that is another Statement.
 * A symbol table ENTRY pointer may be provided, as well.
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class ForStatement: public Statement {

private:
  const char  *lhsName; // name on the lhs of the assignment (init)
  ENTRY       *entry;   // lhs of the assignment (init)
  Expression  *index;   // the index (subscript) expression; entry must be of an array type
  Expression  *init;    // the initialization expression
  Expression  *term;    // the termination expression
  Expression  *upd;     // the update expression
  Statement   *body;    // the for body stmt

public:

  /** 
   * Create a new ForStatement object.
   * 
   * @param lno a source code line number with the for statement (should be the beginning of the for statement)
   * @param n a string representing the name on the left-hand side of the initialization expression
   * @param i a pointer to an Expression representing the initialization expression
   * @param t a pointer to an Expression representing the termination expression
   * @param u a pointer to an Expression representing the update expression
   * @param b a pointer to a Statement representing the for statement body
   * @throws AstException if either the n, i, t, u, or b argument is NULL
   */
  ForStatement( int lno, const char *n, Expression *i, Expression *t, Expression *u, Statement *b );

  /** 
   * Create a new ForStatement object.
   * 
   * @param lno a source code line number with the for statement (should be the beginning of the for statement)
   * @param n a string representing the name on the left-hand side of the initialization expression
   * @param x a pointer to an Expression representing the index (subscript) expression, if the left hand side is an array elemnt
   * @param i a pointer to an Expression representing the initialization expression
   * @param t a pointer to an Expression representing the termination expression
   * @param u a pointer to an Expression representing the update expression
   * @param b a pointer to a Statement representing the for statement body
   * @throws AstException if either the n, x, i, t, u, or b argument is NULL
   */
  ForStatement( int lno, const char *n, Expression *x, Expression *i, Expression *t, Expression *u, Statement *b );

  /**
  * Return the identifier (variable, field, or parameter) on the left hand side of this assignment statement.
  *
  * @return a string representing the left-hand side identifier.
  */
  const char *getLHSName()
  {
    return lhsName;
  }

  /**
  * Return a pointer to the symbol table entry representing the left-hand side of the initialization expression.
  *
  * @return ENTRY pointer to the symbol table.
  */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the symbol table ENTRY representing the left-hand side of the initialization expression.
   * 
   * @param e an ENTRY pointer to the new symbol table ENTRY
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /**
  * Return the index (subscript) Expression, if the left hand side is an array element.
  *
  * @return pointer to an Expression representing the array index (subscript).
  */
  Expression *getIndex()
  {
    return index;
  }

  /**
  * Return the initialization Expression of this for statement
  *
  * @return pointer to an Expression representing the initialization Expression 
  */
  Expression *getInit()
  {
    return init;
  }

  /**
  * Return the termination Expression of this for statement
  *
  * @return pointer to an Expression representing the termination Expression 
  */
  Expression *getTerm()
  {
    return term;
  }

  /**
  * Return the update Expression of this for statement
  *
  * @return pointer to an Expression representing the update Expression 
  */
  Expression *getUpd()
  {
    return upd;
  }

  /**
  * Return the body of this for statement
  *
  * @return pointer to a Statement representing the body of the for statement
  */
  Statement *getBodyStatement()
  {
    return body;
  }

  /** 
   * Set the the body of this for statement
   * 
   * @param b a pointer to a Statement representing the body of the for statement
   * @throws AstException if the argument is NULL
   */
  void setBodyStatement( Statement *b )
  {
    if( b != NULL )
      body = b;
    else
      throw AstException( "ForStatement::setBodyStatement: NULL while statement body\n" );
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
