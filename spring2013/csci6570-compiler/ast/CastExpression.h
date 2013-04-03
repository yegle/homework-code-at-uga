// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __CASTEXPRESSION_H__
#define __CASTEXPRESSION_H__


#include "AstException.h"
#include "AstVisitor.h"
#include "Expression.h"






/**
 * @brief This class represents a TinyJava type cast expression.
 * 
 * A cast expression is represented by:
 * - #the resulting type (an integer, as defined in the AstNode class)
 * - #the Expression whose result should be type cast.
 */
class CastExpression: public Expression {

private:
  int          castTo;   // the intended cast type
  Expression  *operand;  // the operand expression

public:

  // create a unary expression node
  //	  ct is the resulting type
  /** 
   * Create a new CastExpression object.
   * 
   * @param lno a source code line number with the type cast
   * @param castToType the resulting type (as defined in the AstNode class)
   * @param arg the argument expression to be type cast.
   * @throws AstException if the Expression argument is NULL
   */
  CastExpression( int lno, int castToType, Expression *arg );

  /** 
   * Return the type of the tape cast expression.
   * 
   * @return int value representing the type (as defined in the AstNode class)
   */
  int getCastToType()
  {
    return castTo;
  }

  /** 
   * Set the cast-to type.
   * 
   * @param ct cast to be typed to, as defined in the AstNode class
   */
  void setCastToType( int ct )
  {
    castTo = ct;
  }

  /** 
   * Return the operand expression.
   * 
   * @return an Expression node representing the operand expression.
   */
  Expression *getOperand()
  {
    return operand;
  }

  /** 
   * Set the operand expression.
   * 
   * @param arg the new Expression node to be set as the operand.
   * @throws AstException if the Expression argument is NULL
   */  
  void setOperand( Expression *arg )
  {
    if( arg != NULL )
      operand = arg;
    else
      throw AstException( "CastExpression::setOperand: NULL operand\n" );
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
