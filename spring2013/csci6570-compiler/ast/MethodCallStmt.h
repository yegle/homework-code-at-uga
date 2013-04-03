// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __METHODCALLSTMT_H__
#define __METHODCALLSTMT_H__



#include "Statement.h"
#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"





/**
 * @brief This class represents a TinyJava method call statement.
 * 
 * A method call statement node is a "wrapper" class used to
 * represent a method call expression as a statement.
 */
class MethodCallStatement: public Statement {

private:
  Expression *expr;		// the method call expression
  
public:

  /** 
   * Createa a new MethodCallStatement object.
   * 
   * @param lno a source code line number with the method call statement.
   * @param e a pointer to an Expression representing the MethodCallExpression
   * @throws AstException if the e (Expression) argument is NULL or if it is not a MethodCallExpression
   */
  MethodCallStatement( int lno, Expression *e )
  {
    setKind( SMCALL );
    setLineNo( lno );
    expr = NULL;
    if( e != NULL ) {
      if( e->getKind() == EMCALL )
	expr = e;
      else
	throw AstException( "MethodCallStatement::MethodCallStatement: expression is not a method call expression\n" );
    }
    else
      throw AstException( "MethodCallStatement::MethodCallStatement: NULL expression\n" );
  }

  /** 
   * Return the MethodCallExpression of this MethodCallStatement
   *
   * @return a pointer to an Expression representing the MethodCallExpression
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
