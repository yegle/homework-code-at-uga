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

ParameterEntry::ParameterEntry(const char* name, int parameter_type){
    this->name = string(name);
    this->parameter_type = parameter_type;
    this->kind = AstNode::DPARAMETER;
}

ParameterEntry::ParameterEntry(ParameterDeclaration* param_d){
    this->name = string(param_d->getName());
    this->parameter_type = param_d->getType();
    //switch(param_d->getType()){
    //    case AstNode::TINT:
    //    case AstNode::TFLOAT:
    //        this->parameter_type = param_d->getType();
    //        break;
    //    case AstNode::TSTRINGA:
    //        this->parameter_type = AstNode::TSTRING;
    //        break;
    //    default:
    //        throw string("Wrong type initing from ParameterDeclaration.");
    //        break;
    //}
    this->kind = AstNode::DPARAMETER;
}


VariableEntry::VariableEntry(VariableDeclaration* variable_d){
    this->name = string(variable_d->getName());
    switch(variable_d->getType()){
        case AstNode::TINT:
        case AstNode::TFLOAT:
        case AstNode::TINTA:
        case AstNode::TFLOATA:
        case AstNode::TSTRINGA:
            this->variable_type = variable_d->getType();
            break;
        default:
            cout << AstNode::type2string(variable_d->getType()) <<endl;
            cout << variable_d->getLineNo();
            throw string("Wrong type initing from VariableDeclaration.");
            break;
    }
    this->kind = AstNode::DVARIABLE;
}

int ParameterEntry::get_parameter_type(){
    return this->parameter_type;
}

SymbolTable::SymbolTable(){
    this->current_scope = NULL;
    this->current_method_name = string("");
    //this->package_scope = new Scope("package");
    //this->class_scope = new Scope("class");
    //this->method_scope = new Scope("method");
    //this->simpleio_scope = new Scope("simpleio");
    this->scopes = new map<string, Scope*>();
    this->scopes->insert(this->scopes->end(), pair<string, Scope*>(string("package"), new Scope("package")));
    this->scopes->insert(this->scopes->end(), pair<string, Scope*>(string("class"), new Scope("class")));
    this->scopes->insert(this->scopes->end(), pair<string, Scope*>(string("simpleio"), new Scope("simpleio")));

    MethodEntry* readInt_entry = new MethodEntry("readInt", AstNode::TINT);
    MethodEntry* readFloat_entry = new MethodEntry("readFloat", AstNode::TFLOAT);
    MethodEntry* println_entry = new MethodEntry("println", AstNode::TVOID);
    MethodEntry* printInt_entry = new MethodEntry("printInt", AstNode::TVOID);
    MethodEntry* printFloat_entry = new MethodEntry("printFloat", AstNode::TFLOAT);
    MethodEntry* printString_entry = new MethodEntry("printString", AstNode::TFLOAT);

    vector<ParameterEntry*>* pe_list_1 = new vector<ParameterEntry*>();
    pe_list_1->push_back(new ParameterEntry("ival", AstNode::TINT));
    printInt_entry->setParameters(pe_list_1);

    vector<ParameterEntry*>* pe_list_2 = new vector<ParameterEntry*>();
    pe_list_2->push_back(new ParameterEntry("fval", AstNode::TFLOAT));
    printFloat_entry->setParameters(pe_list_2);

    vector<ParameterEntry*>* pe_list_3 = new vector<ParameterEntry*>();
    pe_list_3->push_back(new ParameterEntry("sval", AstNode::TSTRING));
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

    //cout << " install: " << entry->get_name() << " to scope " << current_scope->get_name() << endl;
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
    if(this->list->find(entry->get_name()) != this->list->end()){
        string message = string("Duplicate entry when installing entry " + entry->get_name()+ " in scope " + this->name + ".");
        throw message;
    }
    this->list->insert(this->list->end(), pair<string, Entry*>(entry->get_name(), entry));
}

void Scope::clear(){
    this->entry_list->clear();
    this->list->clear();
}

Entry* Scope::lookup(string name){
    if (this->list->find(name) == this->list->end()){
        return NULL;
    }
    else{
        return this->list->find(name)->second;
    }
}

Scope* SymbolTable::get_scope(){
    return this->current_scope;
}

Scope* SymbolTable::get_scope(const char* scope_name){
    map<string, Scope*>::iterator ret = this->scopes->find(string(scope_name));
    if (ret == this->scopes->end()){
        return NULL;
    }
    else {
        return ret->second;
    }
    //string scope_name_string = string(scope_name);
    //if (scope_name_string == string("simpleio")){
    //    return this->simpleio_scope;
    //}
    //else if (scope_name_string == string("package")){
    //    return this->package_scope;
    //}
    //else if (scope_name_string == string("class")){
    //    return this->class_scope;
    //}
    //else if (scope_name_string == string("method")){
    //    return this->method_scope;
    //}
    //else{
    //    throw string("unknow scope name in SymbolTable::get_scope");
    //}
}

//void SymbolTable::open_scope(){
//    if (this->current_scope == NULL){
//        this->current_scope = this->scopes->get(string("package"));
//    }
//    else if (this->current_scope == this->package_scope){
//        this->current_scope = this->scopes->get(string("class"));
//    }
//    else if(this->current_scope == this->class_scope){
//        this->current_scope = this->scopes->get(string("method"));
//    }
//    else if(this->current_scope == this->method_scope){
//        //this->method_scope->clear();
//        this->current_scope = this->method_scope;
//    }
//    return;
//}

void SymbolTable::use_scope(const char* scope_name){
    map<string, Scope*>::iterator ret = this->scopes->find(string(scope_name));
    if (ret == this->scopes->end()){
        this->current_method_name = string(scope_name);
        //cout << "creating new scope " << string(scope_name) << endl;
        Scope* s = new Scope(scope_name);
        this->scopes->insert(this->scopes->end(), pair<string, Scope*>(string(scope_name), s));
        this->current_scope = s;
    }
    else {
        this->current_scope = ret->second;
    }

    //if(scope_name_string == string("package")){
    //    this->current_scope = this->package_scope;
    //}
    //else if(scope_name_string == string("class")){
    //    this->current_scope = this->class_scope;
    //}
    //else if(scope_name_string == string("simpleio")){
    //    this->current_scope = this->simpleio_scope;
    //}
    //else if(scope_name_string == string("method")){
    //    this->method_scope->clear();
    //    this->current_scope = this->method_scope;
    //}
    //else {
    //    throw string("Unknown scope");
    //}
    //cout << ">>> Change to scope " << scope_name << endl;
    return;
}

Entry* SymbolTable::lookup(const char* name){
    string name_string = string(name);
    string simpleio_prefix = string("SimpleIO.");

    if(!name_string.compare(0,simpleio_prefix.size(), simpleio_prefix)){
        name_string = name_string.substr(simpleio_prefix.size());
        Entry* simpleio_entry = this->scopes->find(string("simpleio"))->second->lookup(name_string);
        return simpleio_entry;
    }

    Entry* method_entry = this->scopes->find(this->current_method_name)->second->lookup(name_string);
    Entry* class_entry = this->scopes->find(string("class"))->second->lookup(name_string);
    Entry* package_entry = this->scopes->find(string("package"))->second->lookup(name_string);

    Scope* cs = this->current_scope;
    Entry* ret = NULL;
    if(cs == this->scopes->find(this->current_method_name)->second){
        ret = (method_entry!=NULL) ? method_entry : (
                    (class_entry!=NULL) ? class_entry : package_entry
                );
    }
    else if(cs == this->scopes->find(string("class"))->second){
        ret = (class_entry!=NULL) ? class_entry : package_entry;
    }
    else if(cs == this->scopes->find(string("package"))->second){
        ret = package_entry;
    }
    //cout << " lookup: " << name << endl;
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
    this->kind = AstNode::DVARIABLE;
}

int VariableEntry::variable_base_type(){
    switch(this->variable_type){
        case AstNode::TINTA:
            return AstNode::TINT;
            break;
        case AstNode::TFLOATA:
            return AstNode::TFLOAT;
            break;
        case AstNode::TSTRINGA:
            return AstNode::TSTRING;
            break;
        default:
            return this->variable_type;
    }
}

int ParameterEntry::parameter_base_type(){
    switch(this->parameter_type){
        case AstNode::TINTA:
            return AstNode::TINT;
            break;
        case AstNode::TFLOATA:
            return AstNode::TFLOAT;
            break;
        case AstNode::TSTRINGA:
            return AstNode::TSTRING;
            break;
        default:
            return this->parameter_type;
    }
}

int FieldEntry::field_base_type(){
    switch(this->field_type){
        case AstNode::TINT:
            return AstNode::TINTA;
            break;
        case AstNode::TFLOAT:
            return AstNode::TFLOATA;
            break;
        case AstNode::TSTRING:
            return AstNode::TSTRINGA;
            break;
        default:
            return this->field_type;
    }
}

int VariableEntry::get_variable_type(){
    return this->variable_type;
}

MethodEntry::MethodEntry(const char* name, int ret_type){
    this->name = string(name);
    //cout << "return_type: " << ret_type << endl;
    this->return_type = ret_type;
    this->kind = AstNode::DMETHOD;
    this->current_index = 0;
    switch(ret_type){
        case AstNode::TVOID:
            this->return_type_spec = string("V");
            break;
        case AstNode::TINT:
            this->return_type_spec = string("I");
            break;
        case AstNode::TFLOAT:
            this->return_type_spec = string("F");
            break;
        case AstNode::TINTA:
            this->return_type_spec = string("[I");
            break;
        case AstNode::TFLOATA:
            this->return_type_spec = string("[F");
        default:
            throw string("unknown return type!!");
    }
}

MethodEntry::MethodEntry(const char* name,
        int return_type,
        vector<ParameterEntry*>* parameter_list = NULL,
        vector<VariableEntry*>* variable_list = NULL){
    this->name = string(name);
    this->return_type = return_type;
    this->parameter_list = parameter_list;
    this->variable_list = variable_list;
    this->kind = AstNode::DMETHOD;
    this->current_index = 0;
}

void MethodEntry::setParameters(vector<Declaration*>* declaration_list){
    vector<ParameterEntry*>* parameter_list = new vector<ParameterEntry*>();

    string arg_specs = string("");
    for(int i=0; i<declaration_list->size(); i++){
        ParameterDeclaration* d = (ParameterDeclaration*)(declaration_list->at(i));
        ParameterEntry* parameter_e = new ParameterEntry(d);
        ParameterDeclaration* d_p = d;
        parameter_list->push_back(parameter_e);
        switch(d->getType()){
            case AstNode::TINT:
                arg_specs += string("I");
                break;
            case AstNode::TFLOAT:
                arg_specs += string("F");
                break;
            case AstNode::TINTA:
                arg_specs += string("[I");
                break;
            case AstNode::TFLOATA:
                arg_specs += string("[F");
                break;
            case AstNode::TSTRING:
                arg_specs += string("Ljava/lang/String;");
                break;
            case AstNode::TSTRINGA:
                arg_specs += string("[Ljava/lang/String;");
                break;
            default:
                throw string("Unknown parameter type!");
        }
        //cout << "type " << AstNode::type2string(d_p->getType())<< ", name " << d_p->getName() << ", function name " << this->get_name()<<endl;
    }
    this->parameter_list = parameter_list;
    this->arg_specs = arg_specs;
}

void MethodEntry::setParameters(vector<ParameterEntry *>* parameter_list){
    this->parameter_list = parameter_list;
    string arg_specs = string("");
    ParameterEntry* e;
    for(int i=0; i < parameter_list->size(); i++){
        e = parameter_list->at(i);
        switch(e->get_parameter_type()){
            case AstNode::TINT:
                arg_specs += string("I");
                break;
            case AstNode::TFLOAT:
                arg_specs += string("F");
                break;
            case AstNode::TINTA:
                arg_specs += string("[I");
                break;
            case AstNode::TFLOATA:
                arg_specs += string("[F");
                break;
            case AstNode::TSTRING:
                arg_specs += string("Ljava/lang/String;");
                break;
            case AstNode::TSTRINGA:
                arg_specs += string("[Ljava/lang/String;");
                break;
            default:
                throw string("Unknown parameter type!");
        }
    }
    this->arg_specs = arg_specs;
}

void MethodEntry::setVariables(vector<Declaration*>* declaration_list){
    vector<VariableEntry*>* variable_list = new vector<VariableEntry*>();
    VariableEntry* variable_e;
    for(int i=0; i<declaration_list->size(); i++){
        Declaration* d = declaration_list->at(i);
        variable_e = new VariableEntry((VariableDeclaration*)d);
        variable_list->push_back(variable_e);
    }
    this->variable_list = variable_list;
}

void MethodEntry::setVariables(vector<VariableEntry *>* variable_list){
    this->variable_list = variable_list;
}

ClassEntry::ClassEntry(const char* name){
    this->name = string(name);
    this->kind = AstNode::DCLASS;
}

FieldEntry::FieldEntry(const char* name, int type, const char* init_value){
    this->name = string(name);
    this->field_type = type;
    this->init_value = string(init_value);
    this->kind = AstNode::DFIELD;
}

int FieldEntry::get_field_type(){
    return this->field_type;
}

vector<ParameterEntry*>* MethodEntry::getParameters(){
    return this->parameter_list;
}

vector<VariableEntry*>* MethodEntry::getVariables(){
    return this->variable_list;
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
