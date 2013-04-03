// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __DECLARATION_H__
#define __DECLARATION_H__


#include "AstNode.h"

/**
 * @brief This is the parent of all declaration Abstract Syntax Tree nodes.
 *
 * This class will not have direct instances. It only serves as the common parent class of:
 *
 * - #ClassDeclaration,
 * - #FieldDeclarations,
 * - #MethodDeclaration,
 * - #ParameterDeclaration, and
 * - #VariableDeclaration
 */
class Declaration: public AstNode {

};




#endif
