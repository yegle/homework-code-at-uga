// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __METHODDECLARATION_H__
#define __METHODDECLARATION_H__


#include <vector>

#include "Declaration.h"
#include "Statement.h"
#include "AstException.h"
#include "AstVisitor.h"




class ENTRY;




/**
 * @brief This class represents a method declaration in a TinyJava class.
 * 
 * A method declaration is represented by:
 * - #the method name (a string)
 * - #whether the method public or not
 * - #the return type of the method (an integer value, as defined in the AstNode class)
 * - #formal parameters of the method (Declaration pointers)
 * - #local variables of the method (Declaration pointers)
 * - #body of the method (a pointer to a Statement)
 *
 *  A pointer to the symbol table ENTRY for the method may also be included.
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class MethodDeclaration: public Declaration {

private:
  const char                 *name;
  bool			      isPublic;  // is this a public method?
  int                         retType;
  std::vector<Declaration *> *params;
  std::vector<Declaration *> *vars;
  ENTRY	                     *entry;     // method symtab entry
  Statement                  *body;      // method body

public:

  /** 
   * Create a new MethodDeclaration object.  The method body is set to NULL.  The symbol table ENTRY pointer is set to NULL.
   * No parameters or local variables are created.
   * 
   * @param lno a source code line number with the method declaration (should be the beginning of the method)
   * @param nm a string representing the method name
   * @param rt a integer value representing the return type, as defined in the AstNode class
   * @throws AstException if the method name is NULL
   */
  MethodDeclaration( int lno, const char *nm, int rt );

  /** 
   * Destroy this object.
   */
  ~MethodDeclaration()
  {
    if( params != NULL )
      delete params;
    if( vars != NULL )
      delete vars;
  }

  /** 
   * Return the method name.
   * 
   * @return string representing the method name.
   */
  const char *getName()
  {
    return name;
  }

  /** 
   * Is the method public?
   * 
   * @return a boolean value representing the method's public visibility status
   */
  bool isPublicMethod()
  {
    return isPublic;
  }

  /** 
   * Set the public visibility status of this method
   * 
   * @param isPubliclyVisible the new public visibility status
   */
  void setPublicMethod( bool isPubliclyVisible )
  {
    isPublic = isPubliclyVisible;
  }

  /** 
   * Return the type of this method declaration node.
   * 
   * @return an integer value representing the method return type, as
   *         defined in the AstNode class.
   */
  int getRetType()
  {
    return retType;
  }

  /** 
   * Set the formal parameters of this method declaration
   * 
   * @param ps a pointer to a std::vector of ParameterDeclaration pointers, each representing a formal parameter declaration
   * @throws AstException if any of the Declaration pointers in the argument std::vector is NULL or is not a ParameterDeclaration
   */
  void setParameters( std::vector<Declaration *> *ps );

  /** 
   * Return all formal parameters of this method.
   * 
   * @return a pointer to a std::vector with pointers referencing ParameterDeclarations of this method
   */
  std::vector<Declaration *> *getParameters()
  {
    return params;
  }

  /** 
   * Set the local variables of this method declaration
   * 
   * @param vs a pointer to a std::vector of VariableDeclaration pointers, each representing a local variable declaration
   * @throws AstException if any of the Declaration pointers in the argument std::vector is NULL or is not a VariableDeclaration
   */
  void setVariables( std::vector<Declaration *> *vs );

  /** 
   * Return all local variables of this method.
   * 
   * @return a pointer to a std::vector with pointers referencing VariableDeclarations of this method
   */
  std::vector<Declaration *> *getVariables()
  {
    return vars;
  }

  /** 
   * Return a pointer to the symbol table ENTRY representing this method declaration.
   * This value may be undefined before the symbol table is created.
   * 
   * @return a pointer to the symbol table entry representing this method declaration.
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the pointer to the symbol table entry representing this method declaration.
   * This value will typically be set during the symbol table construction phase.
   * 
   * @param e the pointer to the symbol table entry representing this method declaration.
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
  }

  /**
  * Return the body of this method
  *
  * @return pointer to a Statement representing the body of the method
  */
  Statement *getBody()
  {
    return body;
  }

  /** 
   * Set the body of thie method
   * 
   * @param b a Statement pointer representing the body of the method; must be a pointer to BlockStatement
   * @throws AstException if the argument b is NULL or if it is not a BlockStatement
   */
  void setBody( Statement *b )
  {
    if( b == NULL || b->getKind() != AstNode::SBLOCK ) {
      throw AstException( "MethodDeclaration::setBody: body is NULL or it is not a BlockStatement\n" );
    }
    else
      body = b;
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
