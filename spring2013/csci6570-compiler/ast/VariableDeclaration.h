// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __VARIABLEDECLARATION_H__
#define __VARIABLEDECLARATION_H__


#include <stdio.h>

#include "Declaration.h"
#include "AstVisitor.h"
#include "AstException.h"


class ENTRY;




/**
 * @brief This class represents a declaration of a local variable in a TinyJava class method.
 * 
 * A variable is represented by:
 * - #variable name - a string
 * - #type - an integer value, as defined in the AstNode class
 * - #initial value - a string representing the initial value (a literal)
 * - #a pointer to the symbol table ENTRY for the variable (which may be initially undefined). 
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
*/
class VariableDeclaration: public Declaration {

private:
  const char  *name;
  int          type;
  const char  *initVal;
  ENTRY	      *entry;     // variable's symtab entry

public:

  /** 
   * Create a new VariableDeclaration object.
   * 
   * @param lno a source code line number with the variable declaration.
   * @param nm a string with the variable name
   * @param tp an integer representing the type, as defined in the AstNode class
   * @param iv a string with the initial value (a literal)
   * @throws AstException if the variable name or the initial value is NULL
   */  
  VariableDeclaration( int lno, const char *nm, int tp, const char *iv )
  {
    setKind( DVARIABLE );
    setLineNo( lno );
    if( nm == NULL )
      throw AstException( "VariableDeclaration::VariableDeclaration: variable name is NULL\n" );
    else
      name = nm;
    type = tp;
    if( iv == NULL )
      throw AstException( "VariableDeclaration::VariableDeclaration: variable initial value is NULL\n" );
    else
      initVal = iv;
    entry = NULL;
  }

  /** 
   * Return the variable name.
   * 
   * @return string representing the variable name.
   */
  const char *getName()
  {
    return name;
  }

  /** 
   * Set the variable name.
   * 
   * @param nm a string representing the new name of the variable.
   */
  void setName( const char *nm )
  {
    if( nm == NULL )
      throw AstException( "VariableDeclaration::setName: variable name is NULL\n" );
    else
      name = nm;
  }

  /** 
   * Return the type of this VariableDeclaration.
   * 
   * @return an integer value representing the type, as
   *         defined in the AstNode class.
   */
  int getType()
  {
    return type;
  }

  /** 
   * Set the new type of this VariableDeclaration.
   * 
   * @param tp an integer value representing the type, as
   *         defined in the AstNode class.
   */
  void setType( int tp )
  {
    type = tp;
  }

  /** 
   * Return the initial value of this VariableDeclaration.
   * 
   * @return a string representing the initial value of the variable.
   */
  const char *getInitValue()
  {
    return initVal;
  }

  /** 
   * Set the new initial value of this VariableDeclaration.
   * 
   * @param iv a string representing the initial value of this variable
   * @throws AstException if the initial value is NULL
   */
  void setInitValue( const char *iv )
  {
    if( iv == NULL )
      throw AstException( "VariableDeclaration::setInitValue: variable initial value is NULL\n" );
    else
      initVal  = iv;
  }

  /** 
   * Return a pointer to the symbol table ENTRY representing this variable
   * This value may be undefined before the symbol table is created.
   * 
   * @return a pointer to the symbol table ENTRY representing this variable
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the pointer to the symbol table ENTRY representing this variable.
   * This value will typically be set during the symbol table construction phase.
   * 
   * @param e the pointer to the symbol table ENTRY representing this variable
   */
  void setEntry( ENTRY *e )
  {
    entry = e;
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
