// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __WHILESTMT_H__
#define __WHILESTMT_H__


#include "Expression.h"
#include "Statement.h"
#include "AstException.h"
#include "AstVisitor.h"



/**
 * @brief  This class represents a TinyJava WHILE statement.
 * 
 * A while statement is represented by:
 * - #a control Expression representation
 * - #the while loop body Statement representation
 */
class WhileStatement: public Statement {

private:
  Expression  *expr;   // the while control expression
  Statement   *body;   // the while body stmt

public:

  /** 
   * Create a new WhileStatement object
   * 
   * @param lno a source code line number with the while statement (should be the beginning of the while statement)
   * @param e a pointer to the control Expression syntax tree
   * @param b a pointer to the Statement body syntax tree
   * @throws AstException if either the e or b argument is NULL
   */
  WhileStatement( int lno, Expression *e, Statement *b )
  {
    setKind( SWHILE );
    setLineNo( lno );
    if( e != NULL )
      expr = e;
    else
      throw AstException( "WhileStatement::WhileStatement: NULL expression\n" );
    if( b != NULL )
      body = b;
    else
      throw AstException( "WhileStatement::WhileStatement: NULL while statement body\n" );
  }

  /**
  * Return the control Expression of this while statement
  *
  * @return pointer to an Expression representing the control expression
  */
  Expression *getExpression()
  {
    return expr;
  }

  /**
  * Return the body of this while statement
  *
  * @return pointer to a Statement representing the body of the while statement
  */
  Statement *getBodyStatement()
  {
    return body;
  }

  /** 
   * Set the the body of this while statement
   * 
   * @param b a pointer to a Statement representing the body of the while statement
   * @throws AstException if the argument is NULL
   */
  void setBodyStatement( Statement *b )
  {
    if( b != NULL )
      body = b;
    else
      throw AstException( "WhileStatement::setBodyStatement: NULL while statement body\n" );
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
