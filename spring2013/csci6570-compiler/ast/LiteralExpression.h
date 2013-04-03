// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __LITERALEXPRESSION_H__
#define __LITERALEXPRESSION_H__



#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"




/**
 * @brief This class represents a TinyJava literal (smallest expression).
 * 
 * The literal is represented as a string.  It is always a leaf in an Abstract Syntax Tree.
 */
class LiteralExpression: public Expression {

private:
  const char  *literal;

public:

  /** 
   * Create a new LiteralExpression object.
   * 
   * @param lno is a source line number with this literal
   * @param lit a string representing the literal value
   * @param tp an integer representing the type, as defined in the AstNode class
   * @throws AstException if the lit argument is NULL
   */
  LiteralExpression( int lno, const char *lit, int tp )
    : Expression( tp )
  {
    setKind( ELITERAL );
    setLineNo( lno );
    if( lit == NULL )
      throw new AstException( "LiteralExpression::LiteralExpression: Literal is NULL\n" );
    else
      literal = lit;
  }

  /** 
   * Return the literal value.
   * 
   * @return a string representing the literal value
   */
  const char *getLiteral()
  {
    return literal;
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
