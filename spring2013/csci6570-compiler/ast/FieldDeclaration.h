// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __FIELDDECLARATION_H__
#define __FIELDDECLARATION_H__


#include <stdio.h>

#include "Declaration.h"
#include "AstVisitor.h"
#include "AstException.h"
#include "LiteralExpression.h"



class ENTRY;





/**
 * @brief This class represents a field declaration in a TinyJava class.
 * 
 * A class field is represented by:
 * - #field name - a string
 * - #type - an integer value, as defined in the AstNode class
 * - #initial value - either a string representing the initial value literal or a LiteralExpression node
* - #a pointer to the symbol table ENTRY for the field may be provided, as well.
 *
 * ENTRY should be defined as a macro, and its value should be a class, which is 
 * the root of the symbol table entry hierarchy. 
 * Its default value is Entry.  The macro should be defined at the beginning of the
 * header file Ast.h.
 */
class FieldDeclaration: public Declaration {

private:
  const char  		*name;	  	// field's name
  int          		 type;	  	// field's type
  const char  		*initVal;	// initial value as a string (lexeme)
  LiteralExpression 	*initLit;	// initial value as a LiteralExpression
  ENTRY	      		*entry;     	// field's symtab entry

public:

  /** 
   * Create a new FieldDeclaration object.
   * 
   * @param lno a source code line number with the field declaration.
   * @param fieldName a string with the field name
   * @param tp an integer representing the type, as defined in the AstNode class
   * @param iv a string with the initial value literal.
   * @throws AstException if the fieldName or the initial value is NULL
   */  
  FieldDeclaration( int lno, const char *fieldName, int tp, const char *iv )
  {
    setKind( DFIELD );
    setLineNo( lno );
    if( fieldName == NULL )
      throw AstException( "FieldDeclaration::FieldDeclaration: field name is NULL\n" );
    else
      name     = fieldName;
    type     = tp;
    if( iv == NULL )
      throw AstException( "FieldDeclaration::FieldDeclaration: field initial value is NULL\n" );
    else
      initVal  = iv;
    initLit = NULL;
    entry   = NULL;
  }

  /** 
   * Create a new FieldDeclaration object.
   * 
   * @param lno a source code line number with the field declaration.
   * @param fieldName a string with the field name
   * @param tp an integer representing the type, as defined in the AstNode class
   * @param il a reference to a LiteralExpression representing the initial value literal
   * @throws AstException if the fieldName or the initial value is NULL
   */  
  FieldDeclaration( int lno, const char *fieldName, int tp, LiteralExpression *il )
  {
    setKind( DFIELD );
    setLineNo( lno );
    if( fieldName == NULL )
      throw AstException( "FieldDeclaration::FieldDeclaration: field name is NULL\n" );
    else
      name     = fieldName;
    type     = tp;
    if( il == NULL )
      throw AstException( "FieldDeclaration::FieldDeclaration: field initial value LiteralExpression is NULL\n" );
    else
      initLit = il;
    initVal = il->getLiteral();
    entry   = NULL;
  }

  /** 
   * Return the field name.
   * 
   * @return string representing the field name.
   */
  const char *getName()
  {
    return name;
  }

  /** 
   * Set the field name.
   * 
   * @param nm a string representing the new name of the field.
   */
  void setName( const char *nm )
  {
    if( nm == NULL )
      throw AstException( "FieldDeclaration::setName: field name is NULL\n" );
    else
      name = nm;
  }

  /** 
   * Return the type of this FieldDeclaration node.
   * 
   * @return an integer value representing the type, as
   *         defined in the AstNode class.
   */
  int getType()
  {
    return type;
  }

  /** 
   * Set the new type of this FieldDeclaration.
   * 
   * @param tp an integer value representing the type, as
   *         defined in the AstNode class.
   */
  void setType( int tp )
  {
    type = tp;
  }

  /** 
   * Return the initial value of this FieldDeclaration.
   * 
   * @return a string representing the initial value of the field.
   */
  const char *getInitValue()
  {
    return initVal;
  }

  /** 
   * Set the new initial value of this FieldDeclaration.
   * 
   * @param iv a string representing the initial value.
   * @throws AstException if the initial value is NULL
   */
  void setInitValue( const char *iv )
  {
    if( iv == NULL )
      throw AstException( "FieldDeclaration::setInitValue: field initial value is NULL\n" );
    else
      initVal  = iv;
  }

  /** 
   * Return the initialization literal of this FieldDeclaration.
   * 
   * @return a reference to a LiteralExpression representing the initial literal of the field.
   */
  LiteralExpression *getInitLiteral()
  {
    return initLit;
  }

  /** 
   * Set the new initialization literal of this FieldDeclaration.
   * 
   * @param il a LiteralExpression reference representing the initialization literal.
   * @throws AstException if the initial value is NULL
   */
  void setInitLiteral( LiteralExpression *il )
  {
    if( il == NULL )
      throw AstException( "FieldDeclaration::setInitValue: field initialization literal is NULL\n" );
    else
      initLit  = il;
  }

  /** 
   * Return a pointer to the symbol table ENTRY representing this field
   * This value may be undefined before the symbol table is created.
   * 
   * @return a pointer to the symbol table ENTRY representing this field
   */
  ENTRY *getEntry()
  {
    return entry;
  }

  /** 
   * Set the pointer to the symbol table entry representing this field.
   * This value will typically be set during the symbol table construction phase.
   * 
   * @param e the pointer to the symbol table ENTRY representing this field
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
