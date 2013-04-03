// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __CLASSDECLARATION_H__
#define __CLASSDECLARATION_H__


#include <vector>


#include "AstException.h"
#include "AstVisitor.h"
#include "Declaration.h"






/**
 * @brief This class represents a TinyJava class declaration.
 * 
 * A class declaration is represented by:
 * - #a string representing the class name
 * - #a list of class members, that is fields and methods, represented by FieldDeclaration and MethodDeclaration nodes, respectively.
 */
class ClassDeclaration: public Declaration {

private:
  const char                 *name;	// name of the declared class
  std::vector<Declaration *> *members;  // class members

public:

  /** 
   * Create a new ClassDeclaration object.
   * 
   * @param lno a source code line number (should be the start of the class declaration).
   * @param className a string with the class name
   */
  ClassDeclaration( int lno, const char *className );

  /** 
   * Return the class name.
   * 
   * @return string representing the class name.
   */
  const char *getName()
  {
    return name;
  }

  /** 
   * Add a new class mamber (either a field or a method).
   * 
   * @param member a Declaration node representing the new class member.  It must be either a pointer to FieldDeclaration or to MethodDeclaration.
   * @throws AstException if the argument is neither a FieldDeclaration nor a MethodDeclaration.
   */
  void addMember( Declaration *member );

  /** 
   * Return the number of class members in this ClassDeclaration node.
   * 
   * @return the number of class members.
   */
  int getNoMembers()
  {
    return members->size();
  }

  /** 
   * Return the i-the class member.
   * 
   * @param pos integer representing the position of the class member requested.
   * @return a pointer to a Declaration representing the requested class member.
   * @throws AstException if there are no members or if the pos value is out of bounds
   */
  Declaration *getMember( int pos )
  {
    if( members == NULL || pos < 0 || pos >= members->size() )
      throw AstException( "ClassDeclaration::getMember: illegal nenber request: no members or position out of bounds\n" );
    return members->at( pos );
  }

  /** 
   * Return all class members.
   * 
   * @return a pointer to a std::vector representing all class members in this class declaration.
   */
  std::vector<Declaration *> *getMembers()
  {
    return members;
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
