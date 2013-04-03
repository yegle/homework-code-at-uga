// Gnu Emacs C++ mode:  -*- C++ -*-


#ifndef __ASTVISITOR_H__
#define __ASTVISITOR_H__



class MethodDeclaration;
class FieldDeclaration;
class ClassDeclaration;
class ParameterDeclaration;
class VariableDeclaration;
class Expression;
class LiteralExpression;
class ReferenceExpression;
class NewExpression;
class MethodCallExpression;
class UnaryExpression;
class CastExpression;
class BinaryExpression;
class Statement;
class AssignStatement;
class BlockStatement;
class EmptyStatement;
class IfStatement;
class MethodCallStatement;
class ReturnStatement;
class WhileStatement;
class ForStatement;

/**
 * @brief This is the parent of all Abstract Syntax Tree visitors; it is an abstract class.
 * 
 * Specialized concrete visitors should be implemented as subclasses of AstVisitor.
 * The AstPrinter class can be used as an example.
 */
class AstVisitor {

public:

  /** 
   * Visit a ClassDeclaration.
   * 
   * @param aDeclNode a class declaration to be visited
   */
  virtual void visit( ClassDeclaration *aDeclNode ) = 0;

  /** 
   * Visit a MethodDeclaration
   * 
   * @param aDeclNode a MethodDeclaration to be visited
   */
  virtual void visit( MethodDeclaration *aDeclNode ) = 0;

  /** 
   * Visit a FieldDeclaration
   * 
   * @param aDeclNode a FieldDeclaration to be visited
   */
  virtual void visit( FieldDeclaration *aDeclNode ) = 0;

  /** 
   * Visit a ParameterDeclaration
   * 
   * @param aDeclNode a ParameterDeclaration to be visited
   */
  virtual void visit( ParameterDeclaration *aDeclNode ) = 0;

  /** 
   * Visit a VariableDeclaration
   * 
   * @param aDeclNode a VariableDeclaration to be visited
   */
  virtual void visit( VariableDeclaration *aDeclNode ) = 0;

  // visit expressions
  /** 
   * Visit a LiteralExpression
   * 
   * @param anExpNode a LiteralExpression to be visited
   */  
  virtual void visit( LiteralExpression *anExpNode ) = 0;

  /** 
   * Visit a ReferenceExpression
   * 
   * @param anExpNode a ReferenceExpression to be visited
   */  
  virtual void visit( ReferenceExpression *anExpNode ) = 0;

  /** 
   * Visit a NewExpression
   * 
   * @param anExpNode a NewExpression to be visited
   */ 
  virtual void visit( NewExpression *anExpNode ) = 0;

  /** 
   * Visit a UnaryExpression
   * 
   * @param anExpNode a UnaryExpression to be visited
   */
  virtual void visit( UnaryExpression *anExpNode ) = 0;

  /** 
   * Visit a BinaryExpression
   * 
   * @param anExpNode a BinaryExpression to be visited
   */
  virtual void visit( BinaryExpression *anExpNode ) = 0;

  /** 
   * Visit a CastExpression
   * 
   * @param anExpNode a CastExpression to be visited
   */ 
  virtual void visit( CastExpression *anExpNode ) = 0;

  /** 
   * Visit a MethodCallExpression
   * 
   * @param anExpNode a MethodCallExpression to be visited
   */ 
  virtual void visit( MethodCallExpression *anExpNode ) = 0;


  // visit statements
  /** 
   * Visit a AssignStatement
   * 
   * @param anStmtNode a AssignStatement to be visited
   */  
  virtual void visit( AssignStatement *anStmtNode ) = 0;

  /** 
   * Visit a WhileStatement 
   * 
   * @param anStmtNode a WhileStatement to be visited
   */
  virtual void visit( WhileStatement *anStmtNode ) = 0;

  /** 
   * Visit a ForStatement
   * 
   * @param anStmtNode a ForStatement to be visited
   */
  virtual void visit( ForStatement *anStmtNode ) = 0;

  /** 
   * Visit a IfStatement
   * 
   * @param anStmtNode a IfStatement to be visited
   */
  virtual void visit( IfStatement *anStmtNode ) = 0;

  /** 
   * Visit a ReturnStatement
   * 
   * @param anStmtNode a ReturnStatement to be visited
   */
  virtual void visit( ReturnStatement *anStmtNode ) = 0;

  /** 
   * Visit a BlockStatement
   * 
   * @param anStmtNode a BlockStatement to be visited
   */
  virtual void visit( BlockStatement *anStmtNode ) = 0;

  /** 
   * Visit a MethodCallStatement
   * 
   * @param anStmtNode a MethodCallStatement to be visited
   */
  virtual void visit( MethodCallStatement *anStmtNode ) = 0;

  /** 
   * Visit a EmptyStatement
   * 
   * @param anStmtNode a EmptyStatement to be visited
   */  
  virtual void visit( EmptyStatement *anStmtNode ) = 0;


};



#endif

