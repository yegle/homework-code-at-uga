// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __BINARYEXPRESSION_H__
#define __BINARYEXPRESSION_H__



#include "AstException.h"
#include "Expression.h"
#include "AstVisitor.h"





/**
 * @brief This class represents a TinyJava binary expression involving a binary operator and 2 operand expressions.
 * 
 * A binary expression is represented by:
 * - #an operator (which may be an arithmetic, relational, or equality operator)
 * - #a left expression subtree representing the left operand, and
 * - #a right expression subtree representing the right operand.
 */
class BinaryExpression: public Expression {

private:
  int          oper;          // the operator used in this expression
  Expression  *leftOperand;   // the left operand expression
  Expression  *rightOperand;  // the right operand expression

public:

  /** 
   * Create a new BinaryExpression object.
   * 
   * @param lno source code line number with the binary expression.
   * @param op an int value representing the operator; it should be one of the constants defined in the AstNode class
   * @param left an Expression representing the left operand
   * @param right an Expression representing the right operand
   * @throws AstException if either the left operand or the right operand is NULL
   */
  BinaryExpression( int lno, int op, Expression *left, Expression *right );

  /** 
   * Return the operator of this binary expression.
   * 
   * @return operator as defined in the AstNode class.
   */
  int getOperator()
  {
    return oper;
  }

  /** 
   * Return the left operand of this binary expression.
   * 
   * @return the Expression which is the left operand.
   */  
  Expression *getLeftOperand()
  {
    return leftOperand;
  }

  /** 
   * Return the right operand of this binary expression.
   * 
   * @return the Expression which is the right operand.
   */  
  Expression *getRightOperand()
  {
    return rightOperand;
  }

  /** 
   * Set the left operand expression of this binary expression.
   * 
   * @param left the expression to be the new left operand.
   * @throws AstException if the argument is NULL
   */
  void setLeftOperand( Expression *left )
  {
    if( left != NULL )
      leftOperand = left;
    else
      throw AstException( "BinaryExpression::setLeftOperand: NULL operand\n" );
  }

  /** 
   * Set the right operand expression of this binary expression.
   * 
   * @param right the expression to be the new right operand.
   * @throws AstException if the argument is NULL
   */
  void setRightOperand( Expression *right )
  {
    if( right != NULL )
      rightOperand = right;
    else
      throw AstException( "BinaryExpression::setRightOperand: NULL operand\n" );
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
