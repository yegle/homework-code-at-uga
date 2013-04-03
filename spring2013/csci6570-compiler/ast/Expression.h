// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__



#include "AstNode.h"


/**
 * @brief This is the parent of all expression Abstract Syntax Tree nodes.
 *
 * This class will not have direct instances.  
 * Each Expression has a type, represented as an integer value, 
 * as defined in the AstNode class.
 *
 * The Expression class serves as the common parent class of:
 *
 * - #ReferenceExpression
 * - #LiteralExpression
 * - #NewExpression
 * - #MethodCallExpression
 * - #CastExpression
 * - #UnaryExpression, and
 * - #BinaryExpression
 */
class Expression: public AstNode {

private:
  int   type;     // expression type (the type of the value produced)

public:

  /** 
   * Create a new Expression object.  The type will be set to TINT.
   * 
   */
  Expression()
  {
    type = TINT;
  }

  /** 
   * Create a new Expression object.
   * 
   * @param tp the type of the Expression.  It must be an integer value, as 
   *           defined in the AstNode class.
   * 
   */
  Expression( int tp )
  {
    type = tp;
  }

  /** 
   * Return the type of this Expression node.
   * 
   * @return an integer value representing the type, as
   *         defined in the AstNode class.
   */
  int getType()
  {
    return type;
  }

  /** 
   * Set the type of this Expression node.
   * 
   * @param tp an integer value representing the type, as
   *           defined in the AstNode class.
   */
  void setType( int tp )
  {
    type = tp;
  }

};




#endif
