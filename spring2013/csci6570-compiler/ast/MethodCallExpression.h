// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __METHODCALLEXPRESSION_H__
#define __METHODCALLEXPRESSION_H__



#include <vector>

#include "Expression.h"
#include "AstVisitor.h"
#include "AstException.h"




class ENTRY;



/**
 * @brief This class represents a TinyJava method call expression.
 * 
 * A nethod call expression is represented by:
 * - #class name, if the referenced method is a static method in a different class
 * - #method name; a pointer to the symbol table ENTRY for the 
 *   name may also be included.
 * - #call arguments
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class MethodCallExpression: public Expression {

private:
  const char		     *className;   // used only if a qualified names has been used
  const char		     *methodName;
  ENTRY                      *entry;
  std::vector<Expression *>  *arguments;   // method arguments

public:

  /** 
   * Create a new MethodCallExpression object.  The default type of this Expression is set to AstNode::TINT.
   * 
   * @param lno a source line number with this method call expression
   * @param className a string representing the class name with the method (if this is a qualified name)
   * @param methodName a string representing the method name
   * @param args a std::vector of Expression pointers, each representing an argument expression
   * @throws AstException if the methodName is NULL
   */
  MethodCallExpression( int lno, const char *className, const char *methodName, std::vector<Expression *> *args );

  /** 
   * Destroy this object.
   */
  ~MethodCallExpression()
  {
    if( arguments != NULL )
      delete arguments;
  }

  /** 
   * Return the class name of this method call (if a qualified name)
   * 
   * @return a string representing the class name
   */  
  const char *getClassName()
  {
    return className;
  }

  /** 
   * Return the method name of this method call
   * 
   * @return a string representing the method name
   */  
  const char *getMethodName()
  {
    return methodName;
  }

  /** 
   * Return the symbol table entry for this method
   * 
   * @return a pointer to the symbol table entry representing the called method
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the symbol table ENTRY for this method
   * 
   * @param e pointer to a new symbol table ENTRY representing the called method
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /** 
   * Return the numbed of arguments in this method call
   * 
   * @return the numbed of arguments in this method call
   */
  int getNoArguments()
  {
    if( arguments != NULL )
      return arguments->size();
    else
      return 0;
  }

  /** 
   * Return the specific argument Expression
   * 
   * @param pos the argument position; the first argument is at position 0
   * @return a pointer to the Expression representing the requested argument of this method call
   * @throws AstException if the requested argument does not exist, i.e. if the method call 
   *         has no arguments, or if the pos value is outside of the argument number range
   */
  Expression *getArgument( int pos )
  {
    if( arguments == NULL || pos < 0 || pos >= arguments->size() )
      throw AstException( "MethodCallExpression::getArgument: the requested argument does not exist" );
      
    return arguments->at( pos );
  }

  /** 
   * Return all arguments of this MethodCallExpression
   * 
   * @return a pointer to a std::vector of Expression pointers, each representing an argument expression
   */
  std::vector<Expression *> *getArguments()
  {
    return arguments;
  }

  /** 
   * Set the arguments for this MethodCallExpression
   * 
   * @param args a pointer to a std::vector of Expression pointers, each representing an argument expression
   */
  void setArguments( std::vector<Expression *> *args )
  {
    arguments = args;
  }

  /** 
   * Add anther argument to this MethodCallExpression.  The new argument will be the last argument.
   * 
   * @param argument an Expression pointer representing an argument expression
   * @throws AstException if the argument is NULL
   */
  void addArgument( Expression *argument );

  /** 
   * Add anther argument to this MethodCallExpression.  The new argument will be the first argument.
   * 
   * @param argument an Expression pointer representing an argument expression
   * @throws AstException if the argument is NULL
   */
  void prependArgument( Expression *argument );

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
