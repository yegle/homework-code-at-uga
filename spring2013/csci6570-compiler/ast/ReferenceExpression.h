// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __REFERENCEEXPRESSION_H__
#define __REFERENCEEXPRESSION_H__


#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"


class ENTRY;






/**
 * @brief  This class represents an identifier reference in a TinyJava expression; may be an array element 
 *         reference with an index (subscript) expression.
 *
 * A referred identifier may be either a class field,
 * a method parameter, or a local variable.  It may also be a reference to an array element.
 * A ReferenceExpression is usually a leaf in an Abstract Syntax Tree.
 * 
 * A reference expression representation has:
 * - #a string representing the referenced name (a variable, parameter, or a class field); 
 * - #an index (subscript) expression, if the reference is to an array element
 * - #a pointer to the symbol table entry for the name may also be included.
 */
class ReferenceExpression: public Expression {

private:
  const char *name;		// the identifier referenced
  ENTRY      *entry;		// it's symtab entry pointer
  Expression *expr;		// index expression, if it's an array reference

public:

  /** 
   * Create a new ReferenceExpression object.
   * 
   * @param lno is a source line number with the referenced identifier
   * @param n is a string representing the name
   * @throws AstException if the name is NULL
   */
  ReferenceExpression( int lno, const char *n )
  {
    setKind( ENAME );
    setLineNo( lno );
    if( n != NULL )
      name   = n;
    else
      throw AstException( "ReferenceExpression::ReferenceExpression: referenced name is NULL\n" );
    entry  = NULL;
    expr   = NULL;
    setType( 0 );
  }

  /** 
   * Create a new ReferenceExpression object.
   * 
   * @param lno is a source line number with the referenced identifier
   * @param n is a string representing the name
   * @param ex is a pointer to an index (subscript) expression, if it is an array element reference
   * @throws AstException if the name or the expression is NULL
   */
  ReferenceExpression( int lno, const char *n, Expression *ex )
  {
    setKind( ENAME );
    setLineNo( lno );
    if( n != NULL )
      name   = n;
    else
      throw AstException( "ReferenceExpression::ReferenceExpression: referenced name is NULL\n" );
    entry  = NULL;
    if( ex != NULL )
      expr = ex;
    else
      throw AstException( "ReferenceExpression::ReferenceExpression: index (subscript) expression is NULL\n" );
    setType( 0 );
  }

  /**
   * Return the referenced identifier (variable, field, or parameter).
   *
   * @return a string representing the referenced name
   */
  const char *getName()
  {
    return name;
  }

  /**
   * Return the symbol table ENTRY representing the referenced identifier.
   *
   * @return ENTRY pointer to the symbol table representing the referenced identifier
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /**
   * Set the symbol table ENTRY representing the referenced identifier.
   *
   * @param e is the new pointer to the symbol table.
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /**
   * Return the index (subscript) expression, if it is an array element reference.
   *
   * @return Expression pointer representing the array index (subscript).
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
