// Gnu Emacs C++ mode:  -*- C++ -*-



#include <iostream>
#include "AstPrinter.h"

using namespace std;


void 
AstPrinter::visit( MethodDeclaration *aDeclNode )
{
  // verify the integrity of the node
  if( aDeclNode == NULL )
    throw AstException( "AstPrinter::visit: MethodDeclaration: NULL argument" );

  if( aDeclNode->getBody() == NULL )
    throw AstException( "AstPrinter::visit: MethodDeclaration: body is NULL" );

  doIndent( indent );

  cout << AstNode::kind2string( aDeclNode->getKind() ) << ": " << " [" << aDeclNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  cout << AstNode::type2string( aDeclNode->getRetType() ) << " " << aDeclNode->getName() << ": ";

  // print the symbol table entry, if defined
  if( aDeclNode->getEntry() != NULL ) {
    cout << "symbol table reference set";
  }
  else
    cout << "no symbol table reference";
  cout << endl;

  // print (visit) the parameters
  if( aDeclNode->getParameters() != NULL ) {
    for( int i = 0; i < aDeclNode->getParameters()->size(); i++ ) {
      indent++;
      aDeclNode->getParameters()->at( i )->accept( this );
      indent--;
    }
  }

  // print (visit) the variables
  if( aDeclNode->getVariables() != NULL ) {
    for( int i = 0; i < aDeclNode->getVariables()->size(); i++ ) {
      indent++;
      aDeclNode->getVariables()->at( i )->accept( this );
      indent--;
    }
  }

  // print (visit) the body
  indent++;
  aDeclNode->getBody()->accept( this );
  indent--;
}

void
AstPrinter::visit( FieldDeclaration *aDeclNode )
{
  // verify the integrity of the node
  if( aDeclNode == NULL )
    throw AstException( "AstPrinter::visit: FieldDeclaration: NULL argument" );

  doIndent( indent );

  cout <<  AstNode::kind2string( aDeclNode->getKind() ) << ": " << " [" << aDeclNode->getLineNo() << "] " << endl;

  doIndent( indent + 1 );

  cout << aDeclNode->getName() << " " << AstNode::type2string( aDeclNode->getType() ) 
       << " = " << aDeclNode->getInitValue() << ": ";

  // print the symbol table entry, if defined
  if( aDeclNode->getEntry() != NULL ) {
    cout << "symbol table reference set";
  }
  else {
    cout << "no symbol table reference";
  }
  cout << endl;
  cout.flush();
}

void
AstPrinter::visit( ParameterDeclaration *aDeclNode )
{
  // verify the integrity of the node
  if( aDeclNode == NULL )
    throw AstException( "AstPrinter::visit: ParameterDeclaration: NULL argument" );
  
  doIndent( indent );

  cout <<  AstNode::kind2string( aDeclNode->getKind() ) << ": " << " [" << aDeclNode->getLineNo() << "] " << endl;

  doIndent( indent + 1 );

  cout << aDeclNode->getName() << " " << AstNode::type2string( aDeclNode->getType() ) << ": ";

  // print the symbol table entry, if defined
  if( aDeclNode->getEntry() != NULL ) {
    cout << "symbol table reference set";
  }
  else {
    cout << "no symbol table reference";
  }
  cout << endl;
  cout.flush();
}

void 
AstPrinter::visit( VariableDeclaration *aDeclNode )
{
  // verify the integrity of the node
  if( aDeclNode == NULL )
    throw AstException( "AstPrinter::visit: VariableDeclaration: NULL argument" );

  doIndent( indent );

  cout <<  AstNode::kind2string( aDeclNode->getKind() ) << ": " << " [" << aDeclNode->getLineNo() << "] " << endl;

  doIndent( indent + 1 );

  cout << aDeclNode->getName() << " " << AstNode::type2string( aDeclNode->getType() ) 
       << " = " << aDeclNode->getInitValue() << ": ";

  // print the symbol table entry, if defined
  if( aDeclNode->getEntry() != NULL ) {
    cout << "symbol table reference set";
  }
  else {
    cout << "no symbol table reference";
  }
  cout << endl;
  cout.flush();
}

void
AstPrinter::visit( ClassDeclaration *aDeclNode )
{
  // verify the integrity of the node
  if( aDeclNode == NULL )
    throw AstException( "AstPrinter::visit: ClassDeclaration: NULL argument" );

  doIndent( indent );

  cout <<  AstNode::kind2string( aDeclNode->getKind() ) << ": " << aDeclNode->getName() 
       << " [" << aDeclNode->getLineNo() << "] " << endl;
  cout.flush();

  // check if members are present
  if( aDeclNode->getMembers() == NULL ) {
    doIndent( indent + 1 );
    cout << "No members" << endl;
    cout.flush();
  }
  else {
    // print (visit) members
    for( int i = 0; i < aDeclNode->getMembers()->size(); i++ ) {
      // check if this member is really there
      if( aDeclNode->getMember( i ) == NULL )
	throw AstException( "AstPrinter::visit: DCLASS: member is NULL" );
      // now, visit the next class member
      indent++;
      aDeclNode->getMember( i )->accept( this );
      indent--;
    }
  }
}


void
AstPrinter::visit( LiteralExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: NULL argument in LiteralExpression" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );

  cout << anExpNode->getLiteral() << " " 
       << AstNode::type2string( anExpNode->getType() ) << endl;
}

void
AstPrinter::visit( ReferenceExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: ReferenceExpression: NULL argument" );

  doIndent( indent );

  cout << AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  cout << anExpNode->getName() << ": ";

  // print the symbol table info
  if( anExpNode->getEntry() != NULL )
    cout << "symbol table reference set";
  else
    cout << "no symbol table reference";
  cout << endl;

  // print (visit) the index expression, if provided
  if( anExpNode->getExpression() != NULL ) {
    indent = indent + 2;
    anExpNode->getExpression()->accept( this );
    indent = indent - 2;
  }
}

void
AstPrinter::visit( NewExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: NewExpression: NULL argument" );

  if( anExpNode->getExpression() == NULL )
    throw AstException( "AstPrinter::visit: NewExpression: size expression is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << AstNode::type2string( anExpNode->getBaseType() );
  cout << " [" << anExpNode->getLineNo() << "] " << endl;

  // now, increase the indent level and visit, i.e. print the size expression
  indent++;
  anExpNode->getExpression()->accept( this );
  indent--;
}

void
AstPrinter::visit( UnaryExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: UnaryExpression: NULL argument" );

  if( anExpNode->getOperand() == NULL )
    throw AstException( "AstPrinter.visit: UnaryExpression: operand is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  cout << AstNode::operator2string( anExpNode->getOperator() );
  if( anExpNode->isPostfix() )
    cout << " postfix" << endl;
  else
    cout << " prefix" << endl;

  // now, increase the indent level and visit, i.e. print the operand
  indent++;
  anExpNode->getOperand()->accept( this );
  indent--;
}

void
AstPrinter::visit( BinaryExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: BinaryExpression: NULL argument" );

  if( anExpNode->getLeftOperand() == NULL )
    throw AstException( "AstPrinter.visit: EBINARY left operand is NULL" );

  if( anExpNode->getRightOperand() == NULL )
    throw AstException( "AstPrinter.visit: EBINARY right operand is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();
    
  doIndent( indent + 1 );
  cout << AstNode::operator2string( anExpNode->getOperator() )  << endl;
  cout.flush();

  // now, increase the indent level and visit, i.e. print (visit) the left and right operands
  indent++;
  anExpNode->getLeftOperand()->accept( this );
  anExpNode->getRightOperand()->accept( this );
  indent--;
}

void
AstPrinter::visit( CastExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: CastExpression: NULL argument" );

  if( anExpNode->getOperand() == NULL )
    throw AstException( "AstPrinter::visit: ECAST operand is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  cout << AstNode::type2string( anExpNode->getCastToType() ) << endl;
    
  // now, increase the indent level and visit, i.e. print the cast expression
  indent++;
  anExpNode->getOperand()->accept( this );
  indent--;
}

void
AstPrinter::visit( MethodCallExpression *anExpNode )
{
  // verify the integrity of the node
  if( anExpNode == NULL )
    throw AstException( "AstPrinter::visit: NULL argument in MethodCallExpression" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anExpNode->getKind() ) << ": " << " [" << anExpNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  if( anExpNode->getClassName() != NULL ) 
    cout << anExpNode->getClassName() << ".";
  cout << anExpNode->getMethodName() << ": ";

  if( anExpNode->getEntry() != NULL )
    cout << "symbol table reference set";
  else
    cout << "no symbol table reference";
  cout << endl;
  cout.flush();

  // check if any arguments have been provided
  if( anExpNode->getArguments() == NULL ) {
    doIndent( indent + 1 );
    cout << "No arguments" << endl;
  }
  else {
    // visit (print) method call arguments
    for( int i = 0; i < anExpNode->getArguments()->size(); i++ ) {
      indent++;
      anExpNode->getArgument( i )->accept( this );
      indent--;
    }
  }
}

void
AstPrinter::visit( AssignStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: AssignStatement: NULL argument" );
      
  if( anStmtNode->getExpression() == NULL )
      throw AstException( "PrintStatementVisitor::visit: AssignStatement: NULL expression" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  doIndent( indent + 1 );
  cout << anStmtNode->getLHSName() << ": ";

  // print the symbol table entry, if defined
  if( anStmtNode->getEntry() != NULL )
    cout << "symbol table reference set";
  else
    cout << "no symbol table reference";
  cout << endl;

  // print (visit) the index expression, if given
  if( anStmtNode->getIndex() != NULL ) {
    indent = indent + 2;
    anStmtNode->getIndex()->accept( this );
    indent = indent - 2;
  }

  // print (visit) the right-hand side expression
  indent++;
  anStmtNode->getExpression()->accept( this );
  indent--;
}

void
AstPrinter::visit( WhileStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: WhileStatement: NULL argument" );

  if( anStmtNode->getExpression() == NULL )
      throw AstException( "AstPrinter::visit: WhileStatement: expression is NULL" );

  if( anStmtNode->getBodyStatement() == NULL )
      throw AstException( "AstPrinter::visit: WhileStatement: statement is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  // print (visit) the control expression
  indent++;
  anStmtNode->getExpression()->accept( this );
  indent--;

  // now, print (visit) the while statement body
  indent++;
  anStmtNode->getBodyStatement()->accept( this );
  indent--;
}

void
AstPrinter::visit( ForStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: ForStatement: NULL argument" );

  if( anStmtNode->getInit() == NULL )
    throw AstException( "AstPrinter::visit: ForStatement: initiatlization expression is NULL" );

  if( anStmtNode->getTerm() == NULL )
    throw AstException( "AstPrinter::visit: ForStatement: termination expression is NULL" );

  if( anStmtNode->getUpd() == NULL )
    throw AstException( "AstPrinter::visit: ForStatement: update expression is NULL" );

  if( anStmtNode->getBodyStatement() == NULL )
    throw AstException( "AstPrinter::visit: ForStatement: statement is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();
    
  doIndent( indent + 1 );
    
  cout << anStmtNode->getLHSName() << ": ";

  // print the symbol table entry, if defined
  if( anStmtNode->getEntry() != NULL )
    cout << "symbol table reference set";
  else
    cout << "no symbol table reference";
  cout << endl;

  // print (visit) the index expression, if given
  if( anStmtNode->getIndex() != NULL ) {
    indent++;
    anStmtNode->getIndex()->accept( this );
    indent--;
  }

  // print (visit) the initialization expression
  indent++;
  anStmtNode->getInit()->accept( this );
  indent--;

  // print (visit) the termination expression
  indent++;
  anStmtNode->getTerm()->accept( this );
  indent--;

  // print (visit) the update expression
  indent++;
  anStmtNode->getUpd()->accept( this );
  indent--;

  // now, visit the while statement body
  indent++;
  anStmtNode->getBodyStatement()->accept( this );
  indent--;
}

void
AstPrinter::visit( IfStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: IfStatement: NULL argument" );

  if( anStmtNode->getExpression() == NULL )
    throw AstException( "AstPrinter::visit: IfStatement: expression is NULL" );

  if( anStmtNode->getThenStatement() == NULL )
    throw AstException( "AstPrinter::visit: IfStatement: then statement is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  // print (visit) the control expression
  indent++;
  anStmtNode->getExpression()->accept( this );
  indent--;

  // print (visit) the if's then statement
  indent++;
  anStmtNode->getThenStatement()->accept( this );
  indent--;

  // print (visit) the if's else statement, if present
  if( anStmtNode->getElseStatement() != NULL ) {
    indent++;
    anStmtNode->getElseStatement()->accept( this );
    indent--;
  }
}

void
AstPrinter::visit( ReturnStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: NULL argument in ReturnStatement" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  // print (visit) the return value expression
  if( anStmtNode->getExpression() != NULL ) {
    indent++;
    anStmtNode->getExpression()->accept( this );
    indent--;
  }
}

void
AstPrinter::visit( BlockStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: BlockStatement: NULL argument" );

  if( anStmtNode->getStatements() == NULL )
    throw AstException( "AstPrinter::visit: BlockStatement: statements are NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  // visit (print) all statements
  for( int i = 0; i < anStmtNode->getStatements()->size(); i++ ) {

    // check if this Statement is OK to visit
    if( anStmtNode->getStatements()->at( i ) == NULL )
      throw AstException( "AstPrinter::visit: BlockStatement: operand statement at position %d is NULL" );

    // visit the block's next statement
    indent++;
    anStmtNode->getStatements()->at( i )->accept( this );
    indent--;
  }
}

void
AstPrinter::visit( MethodCallStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: MethodCallStatement: NULL argument" );

  if( anStmtNode->getExpression() == NULL )
    throw AstException( "PrintStatementVisitor::visit: MethodCallStatement: MethodCallExpression is NULL" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();

  // print (visit) the MethodCallExpression (MethodCallStatement serves as a wrapper only)
  indent++;
  anStmtNode->getExpression()->accept( this );
  indent--;
}

void
AstPrinter::visit( EmptyStatement *anStmtNode )
{
  // verify the integrity of the node
  if( anStmtNode == NULL )
    throw AstException( "AstPrinter::visit: EmptyStatement: NULL argument" );

  doIndent( indent );

  cout <<  AstNode::kind2string( anStmtNode->getKind() ) << ": " << " [" << anStmtNode->getLineNo() << "] " << endl;
  cout.flush();
}
