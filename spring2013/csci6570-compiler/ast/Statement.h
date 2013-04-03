// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __STATEMENT_H__
#define __STATEMENT_H__



#include "AstNode.h"


/**
 * @brief  This class is the parent of all statement Abstract Syntax Tree nodes.
 *
 * This class will not have direct instances. It only serves as the common parent class of:
 *
 * - #AssignStatement
 * - #BlockStatement
 * - #EmptyStatement
 * - #ForStatement
 * - #IfStatement
 * - #MethodCallStatement
 * - #ReturnStatement
 * - #WhileStatement
 */
class Statement: public AstNode {

};




#endif
