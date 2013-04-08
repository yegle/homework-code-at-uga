using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>
#include <sstream>

#include "SymbolTable.h"

extern int yylineno;

AstNode::AstNodeKind Entry::get_kind(){
    return this->kind;
}

string Entry::get_name(){
    return this->name;
}

Scope::Scope(){
    this->entry_list = new vector<Entry*>();
}

ParameterEntry::ParameterEntry(const char* name, int parameter_type){
    this->name = string(name);
    this->parameter_type = parameter_type;
    this->kind = AstNode::DPARAMETER;
}

ParameterEntry::ParameterEntry(ParameterDeclaration* param_d){
    this->name = string(param_d->getName());
    switch(param_d->getType()){
        case AstNode::TINT:
            this->parameter_type = INT;
            break;
        case AstNode::TFLOAT:
            this->parameter_type = FLOAT;
            break;
        case AstNode::TSTRINGA:
            this->parameter_type = STRING;
            break;
        default:
            throw string("Wrong type initing from ParameterDeclaration.");
            break;
    }
    this->kind = AstNode::DPARAMETER;
}

SymbolTable::SymbolTable(){
    this->current_scope = NULL;
    this->package_scope = new Scope("package");
    this->class_scope = new Scope("class");
    this->method_scope = new Scope("method");
    this->simpleio_scope = new Scope("simpleio");

    MethodEntry* readInt_entry = new MethodEntry("readInt", INT);
    MethodEntry* readFloat_entry = new MethodEntry("readFloat", FLOAT);
    MethodEntry* println_entry = new MethodEntry("println", VOID);
    MethodEntry* printInt_entry = new MethodEntry("printInt", VOID);
    MethodEntry* printFloat_entry = new MethodEntry("printFloat", FLOAT);
    MethodEntry* printString_entry = new MethodEntry("printString", FLOAT);

    vector<ParameterEntry*>* pe_list_1 = new vector<ParameterEntry*>();
    pe_list_1->push_back(new ParameterEntry("ival", INT));
    printInt_entry->setParameters(pe_list_1);

    vector<ParameterEntry*>* pe_list_2 = new vector<ParameterEntry*>();
    pe_list_2->push_back(new ParameterEntry("fval", FLOAT));
    printFloat_entry->setParameters(pe_list_2);

    vector<ParameterEntry*>* pe_list_3 = new vector<ParameterEntry*>();
    pe_list_3->push_back(new ParameterEntry("sval", STRING));
    printString_entry->setParameters(pe_list_3);

    this->use_scope("simpleio");
    this->install(readInt_entry);
    this->install(readFloat_entry);
    this->install(println_entry);
    this->install(printInt_entry);
    this->install(printFloat_entry);
    this->install(printString_entry);
}

void SymbolTable::install(Entry* entry){
    Scope* current_scope = this->get_scope();
    current_scope->install(entry);

    cout << " install: " << entry->get_name() << " to scope " << current_scope->get_name() << endl;
}

Scope::Scope(const char* name){
    this->name = string(name);
    this->entry_list = new vector<Entry *>();
    this->list = new map<string, Entry*>();
    return;
}

string Scope::get_name(){
    return this->name;
}

void Scope::set_name(const char* name){
    this->name = string(name);
}

void Scope::install(Entry* entry){
    this->list->find(string("test"));
    if(this->list->find(entry->get_name()) != this->list->end()){
        string message = string("Duplciate entry when installing entry " + entry->get_name()+ " in scope " + this->name + ".");
        throw message;
    }
    this->list->insert(this->list->end(), pair<string, Entry*>(entry->get_name(), entry));
}

void Scope::clear(){
    this->entry_list->clear();
    this->list->clear();
}

Entry* Scope::lookup(string name){
    map<string, Entry*>::iterator ret = this->list->find(name);
    if (ret == this->list->end()){
        return NULL;
    }
    else{
        return ret->second;
    }
}

Scope* SymbolTable::get_scope(){
    return this->current_scope;
}

Scope* SymbolTable::get_scope(const char* scope_name){
    string scope_name_string = string(scope_name);
    if (scope_name_string == string("simpleio")){
        return this->simpleio_scope;
    }
    else if (scope_name_string == string("package")){
        return this->package_scope;
    }
    else if (scope_name_string == string("class")){
        return this->class_scope;
    }
    else if (scope_name_string == string("method")){
        return this->method_scope;
    }
    else{
        throw string("unknow scope name in SymbolTable::get_scope");
    }
}

void SymbolTable::open_scope(){
    if (this->current_scope == NULL){
        this->current_scope = this->package_scope;
    }
    else if (this->current_scope == this->package_scope){
        this->current_scope = this->class_scope;
    }
    else if(this->current_scope == this->class_scope){
        this->current_scope = this->method_scope;
    }
    else if(this->current_scope == this->method_scope){
        this->method_scope->clear();
        this->current_scope = this->method_scope;
    }
    return;
}

void SymbolTable::use_scope(const char* scope_name){
    string scope_name_string = string(scope_name);

    if(scope_name_string == string("package")){
        this->current_scope = this->package_scope;
    }
    else if(scope_name_string == string("class")){
        this->current_scope = this->class_scope;
    }
    else if(scope_name_string == string("simpleio")){
        this->current_scope = this->simpleio_scope;
    }
    else if(scope_name_string == string("method")){
        this->method_scope->clear();
        this->current_scope = this->method_scope;
    }
    else {
        throw string("Unknown scope");
    }
    cout << ">>> Change to scope " << scope_name << endl;
    return;
}

Entry* SymbolTable::lookup(const char* name){
    string name_string = string(name);
    Entry* method_entry = this->method_scope->lookup(name_string);
    Entry* class_entry = this->class_scope->lookup(name_string);
    Entry* package_entry = this->package_scope->lookup(name_string);
    Entry* simpleio_entry = this->simpleio_scope->lookup(name_string);

    Scope* cs = this->current_scope;
    Entry* ret = NULL;
    if(cs == this->method_scope){
        ret = (method_entry!=NULL) ? method_entry : (
                    (class_entry!=NULL) ? class_entry : package_entry
                );
    }
    else if(cs == this->class_scope){
        ret = (class_entry!=NULL) ? class_entry : package_entry;
    }
    else if(cs == this->package_scope){
        ret = package_entry;
    }
    cout << " lookup: " << name << endl;
    //if (ret == NULL){
    //    cout<< "lookup: " << name << " not found !!!! scope searched: " << this->current_scope->get_name() << endl;
    //}
    return ret;
}

VariableEntry::VariableEntry(
        const char* name,
        int variable_type,
        LiteralExpression* init_expression
        ){
    this->name = string(name);
    this->variable_type = variable_type;
    this->init_expression = init_expression;
}

int VariableEntry::get_variable_type(){
    return this->variable_type;
}

MethodEntry::MethodEntry(const char* name, int return_type){
    this->name = string(name);
    this->return_type = return_type;
}

MethodEntry::MethodEntry(const char* name,
        int return_type,
        vector<ParameterEntry*>* parameter_list = NULL,
        vector<VariableEntry*>* variable_list = NULL){
    this->name = string(name);
    this->return_type = return_type;
    this->parameter_list = parameter_list;
    this->variable_list = variable_list;
}

void MethodEntry::setParameters(vector<Declaration*>* declaration_list){
    vector<ParameterEntry*>* parameter_list = new vector<ParameterEntry*>();
    for(int i=0; i<declaration_list->size(); i++){
        Declaration* d = declaration_list->at(i);
        ParameterEntry* parameter_e = new ParameterEntry((ParameterDeclaration*)d);
        parameter_list->push_back(parameter_e);
    }
    this->parameter_list = parameter_list;
}

void MethodEntry::setParameters(vector<ParameterEntry *>* parameter_list){
    this->parameter_list = parameter_list;
}

ClassEntry::ClassEntry(const char* name){
    this->name = string(name);
}

FieldEntry::FieldEntry(const char* name, yytokentype type, const char* init_value){
    this->name = string(name);
    this->type = type;
    this->init_value = string(init_value);
}

/*
int main(){
    SymbolTable* table = new SymbolTable();
    table->open_scope();

    ParameterEntry* e = new ParameterEntry("name", INT);
    ParameterEntry* f = new ParameterEntry("name", INT);

    ClassEntry* c = new ClassEntry("ClassName");

    try{
        table->install(e);
        table->install(c);
    }
    catch(string e){
        cerr << "ERROR: " << e << endl;
        exit(1);
    }
    Entry* entry = table->lookup("name");
    cout << entry->get_name() << endl;
    entry = table->lookup("ClassName");
    cout << entry->get_name() << endl;
}
*/
