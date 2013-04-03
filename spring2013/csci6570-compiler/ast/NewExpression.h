// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __NEWEXPRESSION_H__
#define __NEWEXPRESSION_H__


#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"




/**
 * @brief This class represents a TinyJava NEW expression (used for array creation).
 * 
 * A NewExpression is represented by:
 * - #the constructed type (i.e. array base type)
 * - #an Expression representing the size of the array to be created
 */
class NewExpression: public Expression {

private:
  int          type;     // the base type of the array to create
  Expression  *operand;  // the array size expression

public:

  /** 
   * Create a new NewExpression object
   * 
   * @param lno a source line number containing the new expression
   * @param tp the type of the created value (array base type), as defined in the AstNode class
   * @param size a pointer to Expression representing the size of the new array
   * @throws AstException if the size Expression is NULL
   */
  NewExpression( int lno, int tp, Expression *size )
  {
    setKind( ENEW );
    setLineNo( lno );
    type = tp;
    if( size != NULL )
      operand = size;
    else
      throw AstException( "NewExpression::NewExpression: NULL size expression\n" );
  }

  /** 
   * Return the type to be created (array base type)
   * 
   * @return an integer value representing the type, as
   *         defined in the AstNode class.
   */
  int getBaseType()
  {
    return type;
  }

  /** 
   * Return the size Expression
   *
   * @return a pointer to Expression representing the array size
   */  Expression *getExpression()
  {
    return operand;
  }

  /** 
   * Set the size Expression 
   * 
   * @param arg is a pointer to an Expression representing the array size
   * @throws AstException if the arg Expression is NULL
   */
  void setExpression( Expression *arg )
  {
    if( arg != NULL )
      operand = arg;
    else
      throw AstException( "NewExpression::setOperand: NULL arg\n" );
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
