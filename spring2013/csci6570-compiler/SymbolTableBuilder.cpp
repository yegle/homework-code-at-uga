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
    table->use_scope("class");
    this->debug(aDeclNode->getLineNo(), "MethodDeclaration");

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
        method_e->setParameters(aDeclNode->getParameters());
    }

    // print (visit) the variables
    if( aDeclNode->getVariables() != NULL ) {
        for( int i = 0; i < aDeclNode->getVariables()->size(); i++ ) {
            aDeclNode->getVariables()->at( i )->accept( this );
        }
        method_e->setVariables(aDeclNode->getVariables());
    }

    // print (visit) the body
    aDeclNode->getBody()->accept( this );
    return;
}

void SymbolTableBuilder::visit( FieldDeclaration *aDeclNode ){
    this->debug(aDeclNode->getLineNo(), "FieldDeclaration");

    if(aDeclNode->getType() != aDeclNode->getInitLiteral()->getType()){
        snprintf(this->buf, 1024,
                "Wrong type of initialization variable %s, variable type %s, init_literal type %s",
                aDeclNode->getName(),
                AstNode::type2string(aDeclNode->getType()),
                AstNode::type2string(aDeclNode->getInitLiteral()->getType())
                );
        this->error(aDeclNode->getLineNo());
    }

    VariableEntry* variable_e = new VariableEntry(
            aDeclNode->getName(),
            aDeclNode->getType(),
            aDeclNode->getInitLiteral()
            );
    table->install(variable_e);
    return;
}
void SymbolTableBuilder::visit( ParameterDeclaration *aDeclNode ){
    this->debug(aDeclNode->getLineNo(), "ParameterDeclaration");

    ParameterEntry* parameter_e = new ParameterEntry(aDeclNode->getName(), aDeclNode->getType());
    parameter_e->set_index(this->current_method->get_current_index());
    this->current_method->inc_current_index();
    table->install(parameter_e);
    return;
}
void SymbolTableBuilder::visit( VariableDeclaration *aDeclNode ){
    this->debug(aDeclNode->getLineNo(), "VariableDeclaration");

    if(aDeclNode->getInitLiteral() == NULL){
        throw AstException( string("No InitLiteral in this VaraibleDeclaration! ") + aDeclNode->getName() );
    }

    int node_type = aDeclNode->getType();

    if(node_type == AstNode::TINTA && aDeclNode->getInitLiteral()->getType() != AstNode::TREF){
        throw AstException( string("Wrong type of initialization variable ") + aDeclNode->getName() +
                ", variable type " + AstNode::type2string(aDeclNode->getType()) + ", expecting NULL"
                );
    }
    else if(node_type != AstNode::TINTA && node_type != aDeclNode->getInitLiteral()->getType()){
        throw AstException( string("Wrong type of initialization variable ") + aDeclNode->getName() +
                ", variable type " + AstNode::type2string(aDeclNode->getType()) + ", init_literal type " +
                AstNode::type2string(aDeclNode->getInitLiteral()->getType())
                );
    }


    //cout <<endl;
    //cout << "VariableDeclaration, " << AstNode::type2string(aDeclNode->getType()) << ", name " << aDeclNode->getName() <<endl;
    VariableEntry* variable_e = new VariableEntry(
            aDeclNode->getName(),
            aDeclNode->getType(),
            aDeclNode->getInitLiteral());
    variable_e->set_index(this->current_method->get_current_index());
    this->current_method->inc_current_index();
    table->install(variable_e);
    return;
}
void SymbolTableBuilder::visit( ClassDeclaration *aDeclNode ){
    table->use_scope("package");
    this->debug(aDeclNode->getLineNo(), "ClassDeclaration");
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
    this->debug(anExpNode->getLineNo(), "LiteralExpression");
    return;
}
void SymbolTableBuilder::visit( ReferenceExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "ReferenceExpression");

    int array_item;
    if (anExpNode->getExpression()!=NULL){
        anExpNode->getExpression()->accept( this );
        array_item = 1;
        //cout << "REFERENCE TO ARRAY ITEM" << endl;
    }
    else{
        array_item = 0;
        //cout << "MAYBE REFERENCE TO ARRAY" << endl;
    }

    Entry* a_entry = table->lookup(anExpNode->getName());
    if(a_entry == NULL){
        snprintf(this->buf, 1024, "No reference to expression %s", anExpNode->getName());
        this->error(anExpNode->getLineNo());
    }
    else{
        AstNode::AstNodeKind k = a_entry->get_kind();
        switch(k){
            case AstNode::DVARIABLE:
                if(array_item){
                    anExpNode->setType(((VariableEntry*)a_entry)->variable_base_type());
                }
                else{
                    anExpNode->setType(((VariableEntry*)a_entry)->get_variable_type());
                }
                break;
            case AstNode::DFIELD:
                if(array_item){
                    anExpNode->setType(((FieldEntry*)a_entry)->field_base_type());
                }
                else{
                    anExpNode->setType(((FieldEntry*)a_entry)->get_field_type());
                }
                break;
            case AstNode::DPARAMETER:
                if(array_item){
                    anExpNode->setType(((ParameterEntry*)a_entry)->parameter_base_type());
                }
                else{
                    anExpNode->setType(((ParameterEntry*)a_entry)->get_parameter_type());
                }
                break;
            default:
                snprintf(this->buf, 1024, "Expected '%s' to be a variable or parameter or field", anExpNode->getName());
                this->error(anExpNode->getLineNo());
                break;
        }
    }
    //cout << AstNode::type2string(anExpNode->getType()) << "asdfasdf"<<endl;
    anExpNode->setEntry(a_entry);
    return;
}
void SymbolTableBuilder::visit( NewExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "NewExpression");
    Expression* index_expression = anExpNode->getExpression();
    int type = index_expression->getType();
    if (type != AstNode::TINT){
        snprintf(this->buf, 1024, "Index expression expected to be AstNode::TINT type, get %s instead", AstNode::type2string(type));
        this->error(anExpNode->getLineNo());
    }
    index_expression->accept( this );
    anExpNode->setType(anExpNode->getBaseType());
    return;
}
void SymbolTableBuilder::visit( UnaryExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "UnaryExpression");
    anExpNode->getOperand()->accept( this );

    int type = anExpNode->getOperand()->getType();
    if (type != AstNode::TINT && type != AstNode::TFLOAT && type != AstNode::TINTA && type != AstNode::TFLOATA){
        snprintf(this->buf, 1024, "The operand of UnaryExpression is supposed to be INT or FLOAT, get %s instead.", AstNode::type2string(type));
        this->error(anExpNode->getLineNo());
    }
    else{
        anExpNode->setType(type);
    }
    return;
}
void SymbolTableBuilder::visit( BinaryExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "BinaryExpression");
    Expression* left = anExpNode->getLeftOperand();
    Expression* right = anExpNode->getRightOperand();
    //cout << "BinaryExpression: left" <<endl;
    left->accept(this);
    //cout << "BinaryExpression: right" <<endl;
    right->accept(this);


    //snprintf(this->buf, 1024, "operator is %s", AstNode::operator2string(anExpNode->getOperator()));
    //this->error(anExpNode->getLineNo());

    //snprintf(this->buf, 1024, "left expression's index expression is %x", ((ReferenceExpression*)left)->getExpression());
    //snprintf(this->buf, 1024, "left expression's type is %s", AstNode::type2string(((ReferenceExpression*)left)->getType()));
    //this->error(anExpNode->getLineNo());
    //
    //
    //snprintf(this->buf, 1024, "left %s, right %s, op %s",
    //        AstNode::type2string(left->getType()),
    //        AstNode::type2string(right->getType()),
    //        AstNode::operator2string(anExpNode->getOperator())
    //        );
    //this->error(anExpNode->getLineNo());
    //
    //cout << AstNode::type2string(left->getType()) << endl;
    //cout << AstNode::type2string(right->getType()) << endl;


    if(left->getType() == AstNode::TSTRING || right->getType() == AstNode::TSTRING){
        snprintf(this->buf, 1024, "String expression not allowed in BinaryExpression");
        this->error(anExpNode->getLineNo());
    }
    else if (left->getKind() == AstNode::ENEW || right->getKind() == AstNode::ENEW){
        snprintf(this->buf, 1024, "Array type not allowed in BinaryExpression");
        this->error(anExpNode->getLineNo());
    }
    else {
        if(
                (left->getType() != AstNode::TINT && left->getType() != AstNode::TFLOAT) ||
                (right->getType() != AstNode::TINT && right->getType() != AstNode::TFLOAT)
                ){
            snprintf(this->buf, 1024, "THIS REALLY HAPPENED!? left type %s, right type %s, op %s, I don't know what to do...",
                    AstNode::type2string(left->getType()),
                    AstNode::type2string(right->getType()),
                    AstNode::operator2string(anExpNode->getOperator())
                    );
            this->error(anExpNode->getLineNo());
        }
        else{
            int op = anExpNode->getOperator();
            if(op!=AstNode::ADDOP && op!=AstNode::SUBOP && op!=AstNode::MULOP && op!=AstNode::DIVOP){
                anExpNode->setType(AstNode::TBOOL);
            }
            else if ( left->getType() == AstNode::TINT && right->getType() == AstNode::TINT) {
                anExpNode->setType(AstNode::TINT);
            }
            else if (
                    (left->getType() == AstNode::TFLOAT && right->getType() == AstNode::TFLOAT) ||
                    (left->getType() == AstNode::TINT && right->getType() == AstNode::TFLOAT) ||
                    (left->getType() == AstNode::TFLOAT && right->getType() == AstNode::TINT)
                    ){
                anExpNode->setType(AstNode::TFLOAT);
            }
        }
    }
    return;
}
void SymbolTableBuilder::visit( CastExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "CastExpression");
    anExpNode->getOperand()->accept( this );
    int cast_to = anExpNode->getCastToType();
    if(cast_to != AstNode::TINT && cast_to != AstNode::TFLOAT){
        snprintf(this->buf, 1024, "CastExpression only supports cast to INT or FLOAT, get CastToType %s instead", AstNode::type2string(cast_to));
        this->error(anExpNode->getLineNo());
    }
    else{
        anExpNode->setType(cast_to);
    }
    return;
}
void SymbolTableBuilder::visit( MethodCallExpression *anExpNode ){
    this->debug(anExpNode->getLineNo(), "MethodCallExpression");

    string method_name = string(anExpNode->getMethodName());

    Entry* e = NULL;

    if(anExpNode->getClassName() !=NULL){
        method_name = string(anExpNode->getClassName())+"."+method_name;
    }

    e = table->lookup(method_name.c_str());

    if(e == NULL){
        snprintf(this->buf, 1024, "Undefined reference to method %s", method_name.c_str());
        this->error(anExpNode->getLineNo());
    }
    else if (e->get_kind()!=AstNode::DMETHOD){
        snprintf(this->buf, 1024, "Reference to %s suggest it's a function, get %s instead from symbol table", method_name.c_str(), AstNode::kind2string(e->get_kind()));
        this->error(anExpNode->getLineNo());
    }

    // check if any arguments have been provided
    if( anExpNode->getArguments() != NULL ) {
        if(anExpNode->getArguments()->size() != ((MethodEntry*)e)->getParameters()->size()){
            snprintf(this->buf, 1024, "Function %s requires %d parameters, get %d instead", method_name.c_str(),
                    anExpNode->getArguments()->size(),
                    ((MethodEntry*)e)->getParameters()->size()
                    );
            this->error(anExpNode->getLineNo());
        }
        // visit (print) method call arguments
        for( int i = 0; i < anExpNode->getArguments()->size(); i++ ) {
            anExpNode->getArgument( i )->accept( this );
            int actual_type = anExpNode->getArgument(i)->getType();
            int suppose_type = ((MethodEntry*)e)->getParameters()->at(i)->get_parameter_type();

            //cout << "|||"<<((MethodEntry*)e)->getParameters()->at(i)->get_name()<<"|||"<<endl;
            //cout << "|||"<<((ReferenceExpression*)anExpNode->getArgument(i))->getName()<<"|||"<<endl;

            if(suppose_type == AstNode::TFLOAT && actual_type == AstNode::TINT){
                ;
            }
            else if (suppose_type == actual_type){
                ;
            }
            else{
                snprintf(this->buf, 1024, "Parameter in location %d has incompatible type. Expecting %s, get %s instead",
                        i+1,
                        AstNode::type2string(suppose_type),
                        AstNode::type2string(actual_type)
                        );
                this->error(anExpNode->getLineNo());
            }
        }
    }

    return;
}
void SymbolTableBuilder::visit( AssignStatement *anStmtNode ){
    this->debug(anStmtNode->getLineNo(), "AssignStatement");

    int left_type;
    int right_type;
    int array_item;

    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
        array_item = 1;
        //cout << "LEFT SIDE IS AN ARRAY ITEM, NOT AN ARRAY" << endl;
    }
    else{
        array_item = 0;
        //cout << "LEFT SIDE IS NOT AN ARRAY ITEM, MAYBE AN ARRAY" << endl;
    }

    anStmtNode->getExpression()->accept( this );

    right_type = anStmtNode->getExpression()->getType();

    VariableEntry* lhs_e = (VariableEntry*) table->lookup(anStmtNode->getLHSName());

    if(lhs_e == NULL){
        snprintf(this->buf, 1024, "Undefined reference to variable %s", anStmtNode->getLHSName());
        this->error(anStmtNode->getLineNo());
    }
    else {
        switch(lhs_e->get_variable_type()){
            case AstNode::TINTA:
                if(array_item){
                    left_type = AstNode::TINT;
                    break;
                }
            case AstNode::TFLOATA:
                if(array_item){
                    left_type = AstNode::TFLOAT;
                    break;
                }
            case AstNode::TSTRINGA:
                if(array_item){
                    left_type = AstNode::TSTRING;
                    break;
                }
            default:
                left_type = lhs_e->get_variable_type();
        }
        if(left_type == AstNode::TFLOAT && right_type == AstNode::TINT){
            ;
        }
        else if (left_type == right_type){
            ;
        }
        else{
            snprintf(this->buf, 1024, "Assign wrong type to variable %s, expcting %s, get %s instead", anStmtNode->getLHSName(),
                    AstNode::type2string(left_type),
                    AstNode::type2string(right_type)
                    );
            this->error(anStmtNode->getLineNo());
        }
    }
    return;
}
void SymbolTableBuilder::visit( WhileStatement *anStmtNode ){
    this->debug(anStmtNode->getLineNo(), "WhileStatement");
    if(anStmtNode->getExpression() == NULL){
        snprintf(this->buf, 1024, "Invalid init expression of WhileStatement");
        this->error(anStmtNode->getLineNo());
    }
    if( anStmtNode->getBodyStatement() == NULL ){
        snprintf(this->buf, 1024, "Invalid statment in WhileStatement");
        this->error(anStmtNode->getLineNo());
    }
    anStmtNode->getExpression()->accept( this );
    anStmtNode->getBodyStatement()->accept( this );
    return;
}
void SymbolTableBuilder::visit( ForStatement *anStmtNode ){
    this->debug(anStmtNode->getLineNo(), "ForStatement");
    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
    }

    anStmtNode->getInit()->accept( this );
    anStmtNode->getTerm()->accept( this );
    anStmtNode->getUpd()->accept( this );
    anStmtNode->getBodyStatement()->accept( this );
    return;
}
void SymbolTableBuilder::visit( IfStatement *anStmtNode ){
    this->debug(anStmtNode->getLineNo(), "IfStatement");
    anStmtNode->getExpression()->accept( this );
    anStmtNode->getThenStatement()->accept( this );
    if( anStmtNode->getElseStatement() != NULL ) {
        anStmtNode->getElseStatement()->accept( this );
    }
    return;
}
void SymbolTableBuilder::visit( ReturnStatement *anStmtNode ){
    this->debug(anStmtNode->getLineNo(), "ReturnStatement");
    if( anStmtNode->getExpression() != NULL ) {
        anStmtNode->getExpression()->accept( this );
    }
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
    return;
}

void SymbolTableBuilder::error(int lineno){
    cout << "ERROR::[" << lineno << "]: " << string(this->buf) <<endl;
}
void SymbolTableBuilder::info(int lineno, string message){
    cout << "INFO::[" << lineno << "]: " << string(message) <<endl;
}
void SymbolTableBuilder::info(int lineno, const char* message){
    cout << "INFO::[" << lineno << "]: " << string(message) <<endl;
}
void SymbolTableBuilder::debug(int lineno, string message){
    //cout << "DEBUG::[" << lineno << "]: " << string(message) <<endl;
}
void SymbolTableBuilder::debug(int lineno, const char* message){
    //cout << "DEBUG::[" << lineno << "]: " << string(message) <<endl;
}
