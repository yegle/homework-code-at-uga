#include <string>
#include <iostream>

using namespace std;

#include "Ast.h"
#include "CodeGen.h"
#include "SymbolTable.h"

extern SymbolTable* table;
extern char current_class_name[1024];


CodeGen::CodeGen(){
    this->indent = 0;
    this->lineno = 0;
    return;
}

void CodeGen::visit( LiteralExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    string str = string(anExpNode->getLiteral());
    char buf[1024];
    switch(anExpNode->getType()){
        case AstNode::TINT:
            if(anExpNode->getLiteral()[1] == '\0'){
                switch(anExpNode->getLiteral()[0]){
                    case '0':
                        str = string("iconst_0");
                        break;
                    case '1':
                        str = string("iconst_1");
                        break;
                    case '2':
                        str = string("iconst_2");
                        break;
                    case '3':
                        str = string("iconst_3");
                        break;
                    case '4':
                        str = string("iconst_4");
                        break;
                    case '5':
                        str = string("iconst_5");
                        break;
                    default:
                        snprintf(buf, 1024, "ldc %s", anExpNode->getLiteral());
                        str = string(buf);
                        break;
                }
                this->write(str);
            }
            else{
                this->ldc(anExpNode->getLiteral());
            }
            break;
        case AstNode::TFLOAT:
        case AstNode::TSTRING:
            this->ldc(anExpNode->getLiteral());
            break;
        case AstNode::TREF:
            this->write(string("aconst_null"));
            break;
        default:
            snprintf(this->buf, 1024, "error! unknown type %s", AstNode::type2string(anExpNode->getType()));
            this->error();
            break;
    }
    return;
}

void CodeGen::visit( MethodDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    //this->debug(aDeclNode->getLineNo(), "MethodDeclaration");
    table->use_scope("class");
    //table->use_scope(aDeclNode->getName());
    char buf[1024];
    string public_spec = string("");
    if(string(aDeclNode->getName()) == string("main")){
        public_spec += string("public ");
    }
    MethodEntry* e = (MethodEntry*)(table->lookup(aDeclNode->getName()));
    if(e == NULL){
        throw string("Unknown method!! Forget to change scope?");
    }
    snprintf(buf, 1024, ".method %sstatic %s/%s(%s)%s",
            public_spec.c_str(),
            aDeclNode->getName(),
            current_class_name,
            e->get_arg_specs().c_str(),
            e->get_return_type_spec().c_str()
    );
    this->write(string(buf));
    this->indent++;
    this->write(string(".limit stack 25"));
    this->write(string(".limit locals 25"));

    table->use_scope(aDeclNode->getName());
    if( aDeclNode->getVariables() != NULL ) {
        for( int i = 0; i < aDeclNode->getVariables()->size(); i++ ) {
            aDeclNode->getVariables()->at( i )->accept( this );
        }
    }
    aDeclNode->getBody()->accept( this );
    this->indent--;
    this->write(string(".end method"));
    return;
}
void CodeGen::visit( FieldDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    this->debug("FieldDeclaration");
    return;
}
void CodeGen::visit( ParameterDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    this->debug("ParameterDeclaration");
    ParameterEntry* e = (ParameterEntry*)(aDeclNode->getEntry());

    switch(aDeclNode->getType()){
        case AstNode::TINT:
            this->istore(e->get_index());
            break;
        case AstNode::TFLOAT:
            this->fstore(e->get_index());
            break;
        case AstNode::TREF:
            char buf[1024];
            this->write("aconst_null");
            this->astore(e->get_index());
            break;
        case AstNode::TINTA:
            this->aload(e->get_index());
            this->iaload();
            break;
        case AstNode::TFLOATA:
            this->aload(e->get_index());
            this->faload();
            break;
        default:
            snprintf(this->buf, 1024, "unknown type of parameter_decl %s", AstNode::type2string(aDeclNode->getType()));
            this->error();
            break;
    }
    return;
    return;
}
void CodeGen::visit( VariableDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    //this->debug(aDeclNode->getLineNo(), "VariableDeclaration");
    aDeclNode->getInitLiteral()->accept(this);
    VariableEntry* e = (VariableEntry*)(aDeclNode->getEntry());

    switch(aDeclNode->getType()){
        case AstNode::TINT:
            this->istore(e->get_index());
            break;
        case AstNode::TFLOAT:
            this->fstore(e->get_index());
            break;
        case AstNode::TREF:
            char buf[1024];
            this->write("aconst_null");
            this->astore(e->get_index());
            break;
        case AstNode::TINTA:
            this->aload(e->get_index());
            this->iaload();
            break;
        case AstNode::TFLOATA:
            this->aload(e->get_index());
            this->faload();
            break;
        default:
            snprintf(this->buf, 1024, "unknown type of local_decl %s", AstNode::type2string(aDeclNode->getType()));
            this->error();
            break;
    }
    return;
}
void CodeGen::visit( ClassDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    this->debug("ClassDeclaration");
    table->use_scope("package");
    char buf[1024];
    snprintf(buf, 1024, "; %s.jsm\n.bytecode 50.0\n.source %s.java\n.class public %s\n.super java/lang/Object\n", aDeclNode->getName(), aDeclNode->getName(), aDeclNode->getName());
    this->write(string(buf));

    table->use_scope("class");
    if(aDeclNode->getMembers()!=NULL){
        for(int i=0; i<aDeclNode->getMembers()->size(); i++){
            aDeclNode->getMember( i )->accept( this );
        }
    }
    return;
}
void CodeGen::visit( ReferenceExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    //this->debug(anExpNode->getLineNo(), "ReferenceExpression");
    VariableEntry* ve;
    ParameterEntry* pe;
    bool is_float;
    int index;
    Entry* e = table->lookup(anExpNode->getName());
    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            is_float = (ve->get_variable_type() == AstNode::TFLOAT) ? true : false;
            index = ve->get_index();
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            is_float = (pe->get_parameter_type() == AstNode::TFLOAT) ? true : false;
            index = pe->get_index();
    }
    if(is_float){
        this->fload(index);
    }
    else{
        this->iload(index);
    }
    return;
}
void CodeGen::visit( NewExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    //this->debug("NewExpression");
    anExpNode->getExpression()->accept( this );
    char buf[1024];
    switch(anExpNode->getBaseType()){
        case AstNode::TINTA:
            snprintf(buf, 1024, "newarray int");
            break;
        case AstNode::TFLOATA:
            snprintf(buf, 1024, "newarray float");
            break;
        default:
            snprintf(this->buf, 1024, "wrong base type of array");
            this->error();
            return;
    }
    this->write(string(buf));
    return;
}
void CodeGen::visit( UnaryExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("UnaryExpression");
    return;
}
void CodeGen::visit( BinaryExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    //this->debug(anExpNode->getLineNo(), "BinaryExpression");
    Expression* left = anExpNode->getLeftOperand();
    Expression* right = anExpNode->getRightOperand();
    bool is_float = (left->getType() == AstNode::TFLOAT) or (right->getType() == AstNode::TFLOAT);

    left->accept(this);
    if(is_float and left->getType() == AstNode::TINT){
        this->i2f();
    }
    right->accept(this);
    if(is_float and right->getType() == AstNode::TINT){
        this->i2f();
    }

    string output = string("UNKNOWN_OP");
    switch(anExpNode->getOperator()){
        case AstNode::ADDOP:
            output = (is_float) ? string("fadd") : string("iadd");
            break;
        case AstNode::SUBOP:
            output = (is_float) ? string("fsub") : string("isub");
            break;
        case AstNode::MULOP:
            output = (is_float) ? string("fmul") : string("imul");
            break;
        case AstNode::DIVOP:
            output = (is_float) ? string("fdiv") : string("idiv");
            break;
        default:
            output += string(AstNode::operator2string(anExpNode->getOperator()));
            break;
    }
    this->write(output);
    return;
}
void CodeGen::visit( CastExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("CastExpression");
    return;
}
void CodeGen::visit( MethodCallExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    //this->debug(anExpNode->getLineNo(), "MethodCallExpression");
    char buf[1024];
    string class_name = string(anExpNode->getClassName());
    string method_name = string(anExpNode->getMethodName());
    if(class_name == string("SimpleIO")){
        method_name = string("SimpleIO.") + method_name;
    }
    MethodEntry* e = (MethodEntry*)(table->lookup(method_name.c_str()));

    if (e == NULL){
        snprintf(this->buf, 1024,
                "failed to find method '%s' definition",
                anExpNode->getMethodName()
                );
        this->error();
        return;
    }
    int size = anExpNode->getNoArguments();
    if(size==0){
        snprintf(buf, 1024, "invokestatic %s/%s()%s",
                anExpNode->getClassName(),
                anExpNode->getMethodName(),
                e->get_return_type_spec().c_str()
                );
        this->write(buf);
    }
    else{
        for(int i=0; i<size; i++){
            anExpNode->getArgument( i )->accept( this );
        }
        snprintf(buf, 1024, "invokestatic %s/%s(%s)%s",
                anExpNode->getClassName(),
                anExpNode->getMethodName(),
                e->get_arg_specs().c_str(),
                e->get_return_type_spec().c_str()
                );
        this->write(buf);
    }
    return;
}
void CodeGen::visit( AssignStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    //this->debug(anStmtNode->getLineNo(), "AssignStatement");
    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
    }
    anStmtNode->getExpression()->accept( this );
    Entry* e = table->lookup(anStmtNode->getLHSName());
    int index;
    bool is_int;
    VariableEntry* ve;
    ParameterEntry* pe;
    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            index = ve->get_index();
            is_int = (ve->get_variable_type() == AstNode::TINT) ? true : false;
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            index = pe->get_index();
            is_int = (pe->get_parameter_type() == AstNode::TINT) ? true : false;
            break;
    }
    if(is_int){
        this->istore(index);
    }
    else{
        this->fstore(index);
    }
    return;
}
void CodeGen::visit( WhileStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("WhileStatement");
    this->start_loop();
    anStmtNode->getExpression()->accept( this );
    anStmtNode->getBodyStatement()->accept( this );
    this->end_loop();
    return;
}
void CodeGen::visit( ForStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("ForStatement");
    Entry* e = table->lookup(anStmtNode->getLHSName());

    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
    }
    anStmtNode->getInit()->accept( this );

    char buf[1024];
    VariableEntry* ve;
    ParameterEntry* pe;

    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            if(anStmtNode->getTerm()->getType() == AstNode::TINT){
                snprintf(buf, 1024, "istore %d", ve->get_index());
            }
            else{
                snprintf(buf, 1024, "fstore %d", ve->get_index());
            }
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            if(anStmtNode->getTerm()->getType() == AstNode::TINT){
                snprintf(buf, 1024, "putstatic XXX I");
            }
            else{
                snprintf(buf, 1024, "putstatic XXX F");
            }
            break;
        default:
            snprintf(this->buf, 1024, "Wrong type of the init expression");
            this->error();
            return;
    }

    this->write(string(buf));

    anStmtNode->getTerm()->accept( this );
    anStmtNode->getUpd()->accept( this );
    anStmtNode->getBodyStatement()->accept( this );
    return;
}

void CodeGen::visit( IfStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("IfStatement");

    anStmtNode->getExpression()->accept(this);
    return;
}
void CodeGen::visit( ReturnStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    //this->debug(anStmtNode->getLineNo(), "ReturnStatement");
    string return_output = string("return");
    if( anStmtNode->getExpression() != NULL ) {
        anStmtNode->getExpression()->accept( this );
        switch(anStmtNode->getExpression()->getType()){
            case AstNode::TINT:
                return_output = string("ireturn");
                break;
            case AstNode::TFLOAT:
                return_output = string("freturn");
                break;
        }
    }
    this->write(return_output);
    return;
}
void CodeGen::visit( BlockStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    //this->debug(anStmtNode->getLineNo(), "BlockStatement");
    for( int i = 0; i < anStmtNode->getStatements()->size(); i++ ) {
        anStmtNode->getStatements()->at( i )->accept( this );
    }
    return;
    return;
}
void CodeGen::visit( MethodCallStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    //this->debug(anStmtNode->getLineNo(), "MethodCallStatement");
    anStmtNode->getExpression()->accept( this );
    return;
}
void CodeGen::visit( EmptyStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    //this->debug(anStmtNode->getLineNo(), "EmptyStatement");
    return;
}
void CodeGen::write(string line){
    for(int i=1; i<=this->indent; i++){
        cout << "\t";
    }
    cout << line <<endl;
}
void CodeGen::error(){
    cout << "ERROR::[" << this->lineno << "]: " << string(this->buf) <<endl;
}
void CodeGen::info(string message){
    cout << "INFO::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::info(const char* message){
    cout << "INFO::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::debug(string message){
    cout << "DEBUG::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::debug(const char* message){
    cout << "DEBUG::[" << this->lineno << "]: " << string(message) <<endl;
}