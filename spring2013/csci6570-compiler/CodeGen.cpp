#include <string>
#include <iostream>
#include <stack>

using namespace std;

#include "Ast.h"
#include "CodeGen.h"

extern SymbolTable* table;
extern char current_class_name[1024];


CodeGen::CodeGen(){
    this->indent = 0;
    this->lineno = 0;
    this->indexes = new stack<int>();
    this->current_index = 0;
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
    this->debug("MethodDeclaration");
    table->use_scope("class");
    //table->use_scope(aDeclNode->getName());
    char buf[1024];
    string public_spec = string("");
    if(string(aDeclNode->getName()) == string("main")){
        public_spec += string("public ");
    }
    MethodEntry* e = (MethodEntry*)(table->lookup(aDeclNode->getName()));
    this->current_method = e;
    if(e == NULL){
        throw string("Unknown method!! Forget to change scope?");
    }
    snprintf(buf, 1024, ".method %sstatic %s(%s)%s",
            public_spec.c_str(),
            aDeclNode->getName(),
            e->get_arg_specs().c_str(),
            e->get_return_type_spec().c_str()
    );
    this->write(string(buf));
    this->indent++;
    this->write(string(".limit stack 25"));
    snprintf(buf, 1024, ".limit locals %d", e->get_locals());
    this->write(string(buf));


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
    char buf[1024];
    FieldEntry* fe = (FieldEntry*)(table->lookup(aDeclNode->getName()));
    //if(fe == NULL){
    //    snprintf(this->buf, 1024, "Cannot find this field entry");
    //    this->error();
    //    return;
    //}
    string field_type;
    switch(fe->get_kind()){
        case AstNode::TINT:
            field_type = string("I");
            break;
        case AstNode::TFLOAT:
            field_type = string("F");
            break;
        case AstNode::TINTA:
            field_type = string("[I");
            break;
        case AstNode::TFLOATA:
            field_type = string("[F");
            break;

    }
    snprintf(buf, 1024, ".field static %s %s",
            aDeclNode->getName(),
            field_type.c_str()
            );
    this->write(string(buf));
    return;
}
void CodeGen::visit( ParameterDeclaration *aDeclNode ){
    this->lineno = aDeclNode->getLineNo();
    this->debug("ParameterDeclaration");
    ParameterEntry* e = (ParameterEntry*)(aDeclNode->getEntry());

    switch(aDeclNode->getType()){
        case AstNode::TINT:
        case AstNode::TFLOAT:
            this->store(e->get_index(), aDeclNode->getType() == AstNode::TINT);
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
    this->debug("VariableDeclaration");
    aDeclNode->getInitLiteral()->accept(this);
    VariableEntry* e = (VariableEntry*)(aDeclNode->getEntry());

    switch(aDeclNode->getType()){
        case AstNode::TINT:
        case AstNode::TFLOAT:
            this->store(e->get_index(), aDeclNode->getType() == AstNode::TINT);
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
    bool has_field = false;
    snprintf(buf, 1024, "; %s.jsm", aDeclNode->getName());
    this->write(string(buf));
    this->write(string(""));
    snprintf(buf, 1024, ".bytecode 50.0");
    this->write(string(buf));
    snprintf(buf, 1024, ".source %s.java", aDeclNode->getName());
    this->write(string(buf));
    snprintf(buf, 1024, ".class public %s", aDeclNode->getName());
    this->write(string(buf));
    snprintf(buf, 1024, ".super java/lang/Object");
    this->write(string(buf));

    this->write(string(""));

    this->current_class = (ClassEntry*)(table->lookup(aDeclNode->getName()));

    table->use_scope("class");

    if(aDeclNode->getMembers()!=NULL){
        Declaration* d;
        for(int i=0; i<aDeclNode->getMembers()->size(); i++){
            d = aDeclNode->getMember( i );
            if(d->getKind() == AstNode::DFIELD){
                has_field = true;
                d->accept(this);
            }
        }
    }

    this->write(string(""));


    if(not this->current_class->has_constructor){
        this->write(".method public <init>()V");
        this->indent++;
        this->write(".limit stack 1");
        this->write(".limit locals 1");
        this->write("aload_0");
        this->write("invokespecial java/lang/Object/<init>()V");
        this->_return();
        this->indent--;
        this->write(".end method");
    }

    this->write(string(""));

    if(aDeclNode->getMembers()!=NULL){
        Declaration* d;
        for(int i=0; i<aDeclNode->getMembers()->size(); i++){
            d = aDeclNode->getMember( i );
            if(d->getKind() != AstNode::DFIELD){
                d->accept( this );
                this->write(string(""));
            }
        }
    }

    if(has_field){
        this->write(".method static ()V");
        this->indent++;
        this->write(".limit stack 1");
        this->write(".limit locals 0");
        Declaration* d;
        FieldEntry* fe;
        FieldDeclaration* fd;
        for(int i=0; i<aDeclNode->getMembers()->size(); i++){
            d = aDeclNode->getMember( i );
            if(d->getKind() == AstNode::DFIELD){
                fd = (FieldDeclaration*)d;
                fe = (FieldEntry*)(table->lookup(fd->getName()));
                this->ldc(fd->getInitLiteral()->getLiteral());
                this->putstatic(fe->get_field_spec());
            }
        }
        this->_return();
        this->indent--;
        this->write(".end method");
    }

    return;
}
void CodeGen::visit( ReferenceExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("ReferenceExpression");

    VariableEntry* ve;
    ParameterEntry* pe;
    FieldEntry* fe;
    bool is_int;
    bool is_array;
    int index;
    char buf[1024];
    Entry* e = table->lookup(anExpNode->getName());
    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            is_int = (anExpNode->getType() == AstNode::TINT) or (anExpNode->getType() == AstNode::TINTA and anExpNode->getExpression() != NULL);
            index = ve->get_index();

            this->load(index, is_int);
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            is_int = (anExpNode->getType() == AstNode::TINT) or (anExpNode->getType() == AstNode::TINTA and anExpNode->getExpression() != NULL);
            is_array = (pe->get_parameter_type() == AstNode::TINTA) or (pe->get_parameter_type() == AstNode::TFLOATA);
            index = pe->get_index();
            if(is_array){
                this->aload(index);
            }
            else{
                this->load(index, is_int);
            }
            break;
        case AstNode::DFIELD:
            fe = (FieldEntry*)e;
            this->getstatic(fe->get_field_spec());
            break;
    }
    if(anExpNode->getExpression()!=NULL){
        anExpNode->getExpression()->accept(this);
        this->aload(is_int);
    }

    return;
}
void CodeGen::visit( NewExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("NewExpression");
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
    anExpNode->getOperand()->accept( this );

    Expression* operand = anExpNode->getOperand();
    ReferenceExpression* re;
    FieldEntry* fe;
    Entry* e;
    int index = -1;
    bool is_int;
    bool is_array_item;
    switch(anExpNode->getOperator()){
        case AstNode::ADDOP:
            break;
        case AstNode::SUBOP:
            this->neg(operand->getType() == AstNode::TINT);
            break;
        case AstNode::INCOP:
        case AstNode::DECOP:
            if(operand->getKind() == AstNode::ENAME){
                re = (ReferenceExpression*)operand;
                e = table->lookup(re->getName());
                is_int = (operand->getType() == AstNode::TINT) or (operand->getType() == AstNode::TINTA);
                is_array_item = (re->getExpression() != NULL);

                if(not is_array_item){
                    switch(e->get_kind()){
                        case AstNode::DVARIABLE:
                            index = ((VariableEntry*)e)->get_index();
                            this->load(index, is_int);
                            this->load(index, is_int);
                            this->inc_dec_ldc(is_int);
                            if(anExpNode->getOperator() == AstNode::INCOP){
                                this->add(is_int);
                            }
                            else{
                                this->store(index, is_int);
                            }
                            break;
                        case AstNode::DPARAMETER:
                            index = ((ParameterEntry*)e)->get_index();
                            is_int = (operand->getType() == AstNode::TINT);
                            this->load(index, is_int);
                            this->load(index, is_int);
                            this->inc_dec_ldc(is_int);
                            if(anExpNode->getOperator() == AstNode::INCOP){
                                this->add(is_int);
                            }
                            else{
                                this->sub(is_int);
                            }
                            this->store(index, is_int);
                            break;
                        case AstNode::DFIELD:
                            fe = (FieldEntry*)e;
                            this->getstatic(fe->get_field_spec());
                            this->getstatic(fe->get_field_spec());
                            this->inc_dec_ldc(is_int);
                            if(anExpNode->getOperator() == AstNode::INCOP){
                                this->add(is_int);
                            }
                            else{
                                this->sub(is_int);
                            }
                            this->putstatic(fe->get_field_spec());
                            break;
                        default:
                            snprintf(this->buf, 1024, "INC/DEC operator with unknown kind of expression");
                            this->error();
                            break;
                    }
                }
                else{
                    switch(operand->getKind()){
                        case AstNode::DVARIABLE:
                            index = ((VariableEntry*)e)->get_index();
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->inc_dec_ldc(is_int);
                            this->add(is_int);
                            this->astore(is_int);
                            break;
                        case AstNode::DPARAMETER:
                            index = ((ParameterEntry*)e)->get_index();
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(index);
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->inc_dec_ldc(is_int);
                            this->add(is_int);
                            this->astore(is_int);
                            break;
                        case AstNode::DFIELD:
                            fe = (FieldEntry*)e;
                            this->getstatic(fe->get_field_spec());
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->getstatic(fe->get_field_spec());
                            re->getExpression()->accept(this);
                            this->getstatic(fe->get_field_spec());
                            re->getExpression()->accept(this);
                            this->aload(is_int);
                            this->inc_dec_ldc(is_int);
                            this->add(is_int);
                            this->astore(is_int);
                            break;
                        default:
                            snprintf(this->buf, 1024, "INC/DEC operator with unknown kind of expression");
                            this->error();
                            break;
                    }
                }

            }
            else{
                snprintf(this->buf, 1024, "Operand of INCOP/DECOP is supposed to be kind of ENAME, got %s instead", AstNode::kind2string(operand->getKind()));
                this->error();
            }
            break;
        default:
            snprintf(this->buf, 1024, "unknow operator %s",
                    AstNode::operator2string(anExpNode->getOperator()));
            this->error();
            return;

    }
    return;
}
void CodeGen::visit( BinaryExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("BinaryExpression");
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

    string output = string("UNKNOWN_OP ");
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
        case AstNode::LTOP:
        case AstNode::GTOP:
        case AstNode::LEOP:
        case AstNode::GEOP:
        case AstNode::EQOP:
        case AstNode::NEOP:
            return;
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
    anExpNode->getOperand()->accept(this);
    switch(anExpNode->getCastToType()){
        case AstNode::TINT:
            if(anExpNode->getOperand()->getType() != AstNode::TINT){
                this->f2i();
            }
            break;
        case AstNode::TFLOAT:
            if(anExpNode->getOperand()->getType() != AstNode::TFLOAT){
                this->i2f();
            }
            break;
    }
    return;
}
void CodeGen::visit( MethodCallExpression *anExpNode ){
    this->lineno = anExpNode->getLineNo();
    this->debug("MethodCallExpression");
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
            if(anExpNode->getArgument(i)->getType() == AstNode::TINT and e->getParameter(i)->get_parameter_type() == AstNode::TFLOAT){
                this->i2f();
            }
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
    this->debug("AssignStatement");
    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
    }
    anStmtNode->getExpression()->accept( this );
    Entry* e = table->lookup(anStmtNode->getLHSName());
    int index;
    bool is_int;
    VariableEntry* ve;
    ParameterEntry* pe;
    FieldEntry* fe;
    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            index = ve->get_index();
            is_int = (ve->get_variable_type() == AstNode::TINT);
            this->store(index, is_int);
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            index = pe->get_index();
            is_int = (pe->get_parameter_type() == AstNode::TINT);

            this->store(index, is_int);
            break;
        case AstNode::DFIELD:
            fe = (FieldEntry*)e;
            this->putstatic(fe->get_field_spec());
            break;
    }
    return;
}
void CodeGen::visit( WhileStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("WhileStatement");
    this->new_middle_label();

    BinaryExpression* be = (BinaryExpression*)(anStmtNode->getExpression());
    int op = be->getOperator();
    Expression* left = be->getLeftOperand();
    Expression* right = be->getRightOperand();

    bool is_int = (left->getType() == AstNode::TINT) and (right->getType() == AstNode::TINT);
    bool is_array = (left->getType() == AstNode::TINTA) or (left->getType() == AstNode::TFLOATA) or (left->getType() == AstNode::TREF) or (right->getType() == AstNode::TINTA) or (right->getType() == AstNode::TFLOATA) or (right->getType() == AstNode::TREF);

    this->middle_label();
    anStmtNode->getExpression()->accept( this );
    this->write_operator(op, is_int, is_array, true);
    anStmtNode->getBodyStatement()->accept( this );
    this->_goto(this->get_middle_label());
    this->end_label();
    return;
}
void CodeGen::visit( ForStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("ForStatement");
    Entry* e = table->lookup(anStmtNode->getLHSName());

    this->new_middle_label();

    if( anStmtNode->getIndex() != NULL ) {
        anStmtNode->getIndex()->accept( this );
    }
    anStmtNode->getInit()->accept( this );

    char buf[1024];
    VariableEntry* ve;
    ParameterEntry* pe;
    FieldEntry* fe;
    BinaryExpression* be = (BinaryExpression*)(anStmtNode->getTerm());
    Expression* left = be->getLeftOperand();
    Expression* right = be->getRightOperand();

    bool is_int = (left->getType() == AstNode::TINT) and (right->getType() == AstNode::TINT);
    bool is_array = (left->getType() == AstNode::TINTA) or (left->getType() == AstNode::TFLOATA) or (left->getType() == AstNode::TREF) or (right->getType() == AstNode::TINTA) or (right->getType() == AstNode::TFLOATA) or (right->getType() == AstNode::TREF);

    int op = be->getOperator();

    switch(e->get_kind()){
        case AstNode::DVARIABLE:
            ve = (VariableEntry*)e;
            if(is_array){
                this->astore(is_int);
            }
            else{
                this->store(ve->get_index(), is_int);
            }
            break;
        case AstNode::DPARAMETER:
            pe = (ParameterEntry*)e;
            if(is_array){
                this->astore(is_int);
            }
            else{
                this->store(pe->get_index(), is_int);
            }
            break;
        case AstNode::DFIELD:
            fe = (FieldEntry*)e;
            this->putstatic(fe->get_field_spec());
        default:
            snprintf(this->buf, 1024, "Wrong type of the init expression");
            this->error();
            return;
    }

    this->middle_label();

    anStmtNode->getTerm()->accept( this );
    this->write_operator(op, is_int, is_array, true);
    anStmtNode->getBodyStatement()->accept( this );
    anStmtNode->getUpd()->accept( this );
    this->_goto(this->get_middle_label());
    this->end_label();
    return;
}

void CodeGen::visit( IfStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("IfStatement");

    this->new_middle_label();
    anStmtNode->getExpression()->accept(this);
    BinaryExpression* be = (BinaryExpression*)(anStmtNode->getExpression());
    Expression* left = be->getLeftOperand();
    Expression* right = be->getRightOperand();
    int op = be->getOperator();
    bool is_int = (left->getType() == AstNode::TINT) and (right->getType() == AstNode::TINT);
    bool is_array = (left->getType() == AstNode::TINTA) or (left->getType() == AstNode::TFLOATA) or (left->getType() == AstNode::TREF) or (right->getType() == AstNode::TINTA) or (right->getType() == AstNode::TFLOATA) or (right->getType() == AstNode::TREF);
    this->write_operator(op, is_int, is_array, true);
    anStmtNode->getThenStatement()->accept( this );
    this->_goto(this->get_end_label());
    this->middle_label();
    if( anStmtNode->getElseStatement() != NULL ) {
        anStmtNode->getElseStatement()->accept( this );
    }
    this->end_label();
    return;
}
void CodeGen::visit( ReturnStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("ReturnStatement");
    if( anStmtNode->getExpression() != NULL ) {
        anStmtNode->getExpression()->accept( this );
        this->_return(anStmtNode->getExpression()->getType() == AstNode::TINT);
    }
    else{
        this->_return();
    }
    return;
}
void CodeGen::visit( BlockStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("BlockStatement");
    for( int i = 0; i < anStmtNode->getStatements()->size(); i++ ) {
        anStmtNode->getStatements()->at( i )->accept( this );
    }
    return;
}
void CodeGen::visit( MethodCallStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("MethodCallStatement");
    anStmtNode->getExpression()->accept( this );
    return;
}
void CodeGen::visit( EmptyStatement *anStmtNode ){
    this->lineno = anStmtNode->getLineNo();
    this->debug("EmptyStatement");
    return;
}
void CodeGen::write(string line){
    for(int i=1; i<=this->indent; i++){
        cout << "\t";
    }
    cout << line <<endl;
}
void CodeGen::error(){
    cerr << "ERROR::[" << this->lineno << "]: " << string(this->buf) <<endl;
}
void CodeGen::info(string message){
    cerr << "INFO::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::info(const char* message){
    cerr << "INFO::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::debug(string message){
    //cout << "DEBUG::[" << this->lineno << "]: " << string(message) <<endl;
}
void CodeGen::debug(const char* message){
    //cout << "DEBUG::[" << this->lineno << "]: " << string(message) <<endl;
}
