// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __IFSTMT_H__
#define __IFSTMT_H__



#include "AstException.h"
#include "Expression.h"
#include "Statement.h"
#include "AstVisitor.h"




/**
 * @brief This class represents a TinyJava IF statement.
 * 
 * An if statement is represented by:
 * - #a control Expression representation
 * - #a then Statement representation
 * - #an else Statement representation, which may be NULL
 */
class IfStatement: public Statement {

private:
  Expression  *expr;       // the if expression
  Statement   *thenStmt;   // the then stmt
  Statement   *elseStmt;   // the else stmt

public:

  /** 
   * Create a new IfStatement object
   * 
   * @param lno a source code line number with the if statement (should be the beginning of the if statement)
   * @param e a pointer to the control Expression syntax tree
   * @param ts a pointer to the THEN Statement syntax tree
   * @param es a pointer to the ELSE Statement syntax tree, which may be NULL if the if statement has no ELSE part
   * @throws AstException if either the e or ts argument is NULL
   */
  IfStatement( int lno, Expression *e, Statement *ts, Statement *es );

  /**
  * Return the control Expression of this if statement
  *
  * @return pointer to an Expression representing the control expression
  */
  Expression *getExpression()
  {
    return expr;
  }

  /**
  * Return the THEN statement of this if statement
  *
  * @return pointer to a Statement representing the THEN statement
  */
  Statement *getThenStatement()
  {
    return thenStmt;
  }

  /**
  * Return the ELSE statement of this if statement
  *
  * @return pointer to a Statement representing the ELSE statement
  */
  Statement *getElseStatement()
  {
    return elseStmt;
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
