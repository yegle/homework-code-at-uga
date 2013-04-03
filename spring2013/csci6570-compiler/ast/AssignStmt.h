// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __ASSIGNSTMT_H__
#define __ASSIGNSTMT_H__


#include <stdlib.h>

#include "AstException.h"
#include "AstVisitor.h"
#include "Expression.h"
#include "Statement.h"


// this is an anonymous declaration of the ENTRY class;
// it should be the root of the symbol table entry hierarchy,
// as explained in class lectures
class ENTRY;



/**
 * @brief This class represents a TinyJava assignment statement.
 * 
 * An assignment statement has:
 * - #a left-hand side, which may be a simple name (a variable, parameter, or a class field), or 
 *   an array reference with a subscript expression.  A pointer to the symbol table ENTRY for the 
 *   name may also be included.
 * - #a right-hand side, which is an expression.
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class AssignStatement: public Statement {

private:
  const char  *lhsName;
  ENTRY       *entry;      // the lhs reference
  Expression  *index;      // the index expression, if lhs is an array element
  Expression  *expr;       // the if expression

public:

  /**
   * Create a assignment statement node, where the left-hand side is a simple variable, 
   * parameter, or a class field.
   * @param lineNo is a source line number with the assignment statement
   * @param lhsName is the name on the left-hand side of this assignment
   * @param rhs is a pointer to an expression node representing the right-hand side expression
   * @throws AstException if the lhs or the rhs is NULL
   */
  AssignStatement( int lineNo, const char *lhsName, Expression *rhs );

  /**
  * Create a assignment statement node, where the left-hand side is a simple variable, 
  * parameter, or a class field.
  * @param lineNo is a source line number.
  * @param lhsName is the name on the left-hand side of this assignment.
  * @param idx is a pointer to an expression node representing the index (subscript) expression.
  * @param rhs is a pointer to an expression node representing the right-hand side expression.
  * @throws AstException if the lhs or the rhs is NULL
  */
  AssignStatement( int lineNo, const char *lhsName, Expression *idx, Expression *rhs );

  /**
   * Return the identifier (variable, field, or parameter) on the left hand side of this assignment statement.
   * @return const char * pointer to the left-hand side identifier.
   */
  const char *getLHSName()
  {
    return lhsName;
  }

  /**
   * Return the symbol table entry representing the left-hand side of this assignment.
   * @return ENTRY pointer to the symbol table.
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /**
   * Set the symbol table entry representing the left-hand side of this assignment.
   *
   * @param e is the new pointer to the symbol table.
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /**
   * Return the index expression, if the left hand side is an array element.
   *
   * @return Expression pointer representing the array index (subscript).
   */
  Expression *getIndex()
  {
    return index;
  }

  /**
   * Return the expression pointer representing the right-hand side of this assignment.
   * @return Expression pointer representing the right-hand side expression.
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
