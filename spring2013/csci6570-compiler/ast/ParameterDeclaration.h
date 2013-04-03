// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __PARAMETERDECLARATION_H__
#define __PARAMETERDECLARATION_H__


#include <stdio.h>

#include "Declaration.h"
#include "AstVisitor.h"


class ENTRY;




/**
 * @brief This class represents a declaration of a formal parameter of a TinyJava class method.
 * 
 * A parameter is represented by:
 * - #parameter name - a string
 * - #type - an integer value, as defined in the AstNode class
 * - #a pointer to the symbol table ENTRY for the variable (which may be initially undefined). 
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
*/
class ParameterDeclaration: public Declaration {

private:
  const char  *name;
  int          type;
  ENTRY	      *entry;     // parameter's symtab entry

public:

  /** 
   * Create a new ParameterDeclaration object.
   * 
   * @param lno a source code line number with the parameter declaration.
   * @param nm a string with the parameter name
   * @param tp an integer representing the type, as defined in the AstNode class
   * @throws AstException if the parameter name is NULL
   */  
  ParameterDeclaration( int lno, const char *nm, int tp )
  {
    setKind( DPARAMETER );
    setLineNo( lno );
    if( nm == NULL )
      throw AstException( "ParameterDeclaration::ParameterDeclaration: parameter name is NULL\n" );
    else
      name     = nm;
    type     = tp;
    entry    = NULL;
  }

  /** 
   * Return the parameter name.
   * 
   * @return string representing the parameter name.
   */
  const char *getName()
  {
    return name;
  }

  /** 
   * Set the parameter name.
   * 
   * @param nm a string representing the new name of the parameter
   */
  void setName( const char *nm )
  {
    if( nm == NULL )
      throw AstException( "ParameterDeclaration::setName: parameter name is NULL\n" );
    else
      name = nm;
  }

  /** 
   * Return the type of this ParameterDeclaration.
   * 
   * @return an integer value representing the type, as
   *         defined in the AstNode class.
   */
  int getType()
  {
    return type;
  }

  /** 
   * Set the new type of this ParameterDeclaration.
   * 
   * @param tp an integer value representing the type, as
   *         defined in the AstNode class.
   */
  void setType( int tp )
  {
    type = tp;
  }

  /** 
   * Return a pointer to the symbol table ENTRY representing this parameter
   * This value may be undefined before the symbol table is created.
   * 
   * @return a pointer to the symbol table ENTRY representing this parameter
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the pointer to the symbol table ENTRY representing this parameter.
   * This value will typically be set during the symbol table construction phase.
   * 
   * @param e the pointer to the symbol table ENTRY representing this parameter
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
