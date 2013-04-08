using namespace std;

#include "Ast.h"
#include "AstNode.h"
#include "AstPrinter.h"
#include "SymbolTable.h"
#include "SymbolTableBuilder.h"

extern SymbolTable* table;

SymbolTableBuilder::SymbolTableBuilder(){
    this->current_class = NULL;
    this->current_method = NULL;
    return;
}
void SymbolTableBuilder::visit( MethodDeclaration *aDeclNode ){
    cout << "MethodDeclaration";

    table->use_scope("class");
    MethodEntry* method_e = new MethodEntry(aDeclNode->getName(), aDeclNode->getRetType());
    this->current_method = method_e;
    table->install(method_e);

    table->use_scope("method");
    table->get_scope()->set_name(aDeclNode->getName());
    // print (visit) the parameters
    if( aDeclNode->getParameters() != NULL ) {
        for( int i = 0; i < aDeclNode->getParameters()->size(); i++ ) {
            aDeclNode->getParameters()->at( i )->accept( this );
        }
    }

    // print (visit) the variables
    if( aDeclNode->getVariables() != NULL ) {
        for( int i = 0; i < aDeclNode->getVariables()->size(); i++ ) {
            aDeclNode->getVariables()->at( i )->accept( this );
        }
    }

    // print (visit) the body
    aDeclNode->getBody()->accept( this );
    return;
}

void SymbolTableBuilder::visit( FieldDeclaration *aDeclNode ){
    cout << "FieldDeclaration" <<endl;
    return;
}
void SymbolTableBuilder::visit( ParameterDeclaration *aDeclNode ){
    cout << "ParameterDeclaration";

    ParameterEntry* parameter_e = new ParameterEntry(aDeclNode->getName(), aDeclNode->getType());
    table->install(parameter_e);
    return;
}
void SymbolTableBuilder::visit( VariableDeclaration *aDeclNode ){
    cout << "VariableDeclaration";

    VariableEntry* variable_e = new VariableEntry(aDeclNode->getName(), aDeclNode->getType(), aDeclNode->getInitValue());
    table->install(variable_e);
    return;
}
void SymbolTableBuilder::visit( ClassDeclaration *aDeclNode ){
    cout << "ClassDeclaration";

    table->use_scope("package");
    ClassEntry* class_e = new ClassEntry(aDeclNode->getName());
    this->current_class = class_e;
    table->install(class_e);

    table->use_scope("class");
    if(aDeclNode->getMembers()!=NULL){
        for(int i=0; i<aDeclNode->getMembers()->size(); i++){
            aDeclNode->getMember( i )->accept( this );
        }
    }
    return;
}
void SymbolTableBuilder::visit( LiteralExpression *anExpNode ){
    cout << "LiteralExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( ReferenceExpression *anExpNode ){
    cout << "ReferenceExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( NewExpression *anExpNode ){
    cout << "NewExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( UnaryExpression *anExpNode ){
    cout << "UnaryExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( BinaryExpression *anExpNode ){
    cout << "BinaryExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( CastExpression *anExpNode ){
    cout << "CastExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( MethodCallExpression *anExpNode ){
    cout << "MethodCallExpression" <<endl;
    return;
}
void SymbolTableBuilder::visit( AssignStatement *anStmtNode ){
    cout << "AssignStatement" <<endl;
    return;
}
void SymbolTableBuilder::visit( WhileStatement *anStmtNode ){
    cout << "WhileStatement" <<endl;
    return;
}
void SymbolTableBuilder::visit( ForStatement *anStmtNode ){
    cout << "ForStatement" <<endl;
    return;
}
void SymbolTableBuilder::visit( IfStatement *anStmtNode ){
    cout << "IfStatement" <<endl;
    return;
}
void SymbolTableBuilder::visit( ReturnStatement *anStmtNode ){
    cout << "ReturnStatement" <<endl;
    return;
}
void SymbolTableBuilder::visit( BlockStatement *anStmtNode ){
    // visit (print) all statements
    for( int i = 0; i < anStmtNode->getStatements()->size(); i++ ) {
        // visit the block's next statement
        anStmtNode->getStatements()->at( i )->accept( this );
    }
    return;
}
void SymbolTableBuilder::visit( MethodCallStatement *anStmtNode ){
    anStmtNode->getExpression()->accept( this );
    return;
}
void SymbolTableBuilder::visit( EmptyStatement *anStmtNode ){
    cout << "EmptyStatement" <<endl;
    return;
}
