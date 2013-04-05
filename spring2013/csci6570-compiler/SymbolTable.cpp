using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>
#include <sstream>

#include "SymbolTable.h"

extern int yylineno;

Kind Entry::get_kind(){
    return this->kind;
}

string Entry::get_name(){
    return this->name;
}

Scope::Scope(){
    this->entry_list = new vector<Entry*>();
}

ParameterEntry::ParameterEntry(const char* name, yytokentype parameter_type){
    this->name = string(name);
    this->parameter_type = parameter_type;
    this->kind = KPARAMETER;
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
            cout<< "asdf" <<param_d->getType() << "asdf"<< endl;
            throw string("Wrong type initing from ParameterDeclaration.");
            break;
    }
    this->kind = KPARAMETER;
}

SymbolTable::SymbolTable(){
    this->current_scope = NULL;
    this->package_scope = new Scope("package");
    this->class_scope = new Scope("class");
    this->method_scope = new Scope("method");
}

void SymbolTable::install(Entry* entry){
    Scope* current_scope = this->get_scope();
    current_scope->install(entry);
}

Scope::Scope(const char* name){
    this->name = string(name);
    this->entry_list = new vector<Entry *>();
    return;
}

string Scope::get_name(){
    return this->name;
}

void Scope::install(Entry* entry){
    cout << entry->get_name() << endl;
    if(this->lookup(entry->get_name())!=NULL){
        string message  = string("Duplciate entry when installing entry " + entry->get_name()+ " in scope " + this->name + ".");
        throw message;
    }

    this->entry_list->push_back(entry);
}

void Scope::clear(){
    this->entry_list->clear();
}

Entry* Scope::lookup(string name){
    Entry* entry;
    for(int i=0; i < this->entry_list->size(); i++){
        entry = this->entry_list->at(i);
        if(entry->get_name() == name){
            return entry;
        }
    }
    return NULL;
}

Scope* SymbolTable::get_scope(){
    return this->current_scope;
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
    else if(scope_name_string == string("method")){
        this->method_scope->clear();
        this->current_scope = this->method_scope;
    }
    else {
        throw string("Unknown scope");
    }
    return;
}

Entry* SymbolTable::lookup(const char* name){
    string name_string = string(name);
    Entry* method_entry = this->method_scope->lookup(name_string);
    Entry* class_entry = this->class_scope->lookup(name_string);
    Entry* package_entry = this->package_scope->lookup(name_string);

    Scope* cs = this->current_scope;
    if(cs == this->method_scope){
        return (method_entry!=NULL) ? method_entry : (
                    (class_entry!=NULL) ? class_entry : package_entry
                );
    }
    else if(cs == this->class_scope){
        return (class_entry!=NULL) ? class_entry : package_entry;
    }
    else if(cs == this->package_scope){
        return package_entry;
    }
}

VariableEntry::VariableEntry(const char* name, yytokentype variable_type, string init_value){
    this->name = string(name);
    this->variable_type = variable_type;
    this->init_value = init_value;
}

MethodEntry::MethodEntry(const char* name, yytokentype return_type){
    this->name = string(name);
    this->return_type = return_type;
}

MethodEntry::MethodEntry(const char* name,
        yytokentype return_type,
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

ClassEntry::ClassEntry(const char* name){
    this->name = string(name);
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
