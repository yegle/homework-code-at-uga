// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __UNARYEXPRESSION_H__
#define __UNARYEXPRESSION_H__


#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"





/**
 * @brief This class represents a TinyJava (prefix or postfix) unary expression involving a unary operator and an operand expression.
 * 
 * A unary expression is represented by:
 * - #an operator (which must be an arithmetic operator)
 * - #information if it is a postfix unary expression
 * - #an operand Expression subtree representing the operator's argument
 */
class UnaryExpression: public Expression {

private:
  int          oper;     // the operator used in this expression
  bool         postfix;  // true iff this is a postfix expression
  Expression  *operand;  // the operand expression

public:

  /** 
   * Create a new UnaryExpression object.
   * 
   * @param lno source code line number with the unary expression.
   * @param op an int value representing the operator; it should be one of the constants defined in the AstNode class
   * @param arg an Expression representing the operand
   * @param post a boolean value indicating if it is a postfix unary expression; the default is false
   * @throws AstException if either the arg operand is NULL
   */
  UnaryExpression( int lno, int op, Expression *arg, bool post = false )
  {
    setKind( EUNARY );
    setLineNo( lno );
    oper         = op;
    postfix      = post;
    setType( TINT );   // set as the default expression type
    if( arg != NULL )
      operand = arg;
    else
      throw AstException( "UnaryExpression::UnaryExpression: operand is NULL\n" );
  }

  /** 
   * Return the operator of this unary expression.
   * 
   * @return operator, as defined in the AstNode class.
   */
  int getOperator()
  {
    return oper;
  }

  /** 
   * Return the operand of this unary expression.
   * 
   * @return the Expression pointer which is the operand expression tree
   */  
  Expression *getOperand()
  {
    return operand;
  }

  /** 
   * Set the operand expression of this unary expression.
   * 
   * @param arg the expression to be the new operand
   * @throws AstException if the argument expression is NULL
   */
  void setOperand( Expression *arg )
  {
    if( arg != NULL )
      operand = arg;
    else
      throw AstException( "UnaryExpression::setOperand: operand is NULL\n" );
  }

  /** 
   * Is it a postfix unary expression?
   * 
   * @return a boolean value indicating if it is a postifx unary expression
   */
  bool isPostfix()
  {
    return postfix;
  }

  /**
  * Accept a visitor to this node.
  * @param aVisitor is a visitor object of type AstVisitor.
  */
  void accept( AstVisitor *aVisitor )
  {
    aVisitor->visit( this );	// now, invoke it
  }

};




#endif
