using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>
#include "SymbolTable.h"

string ParameterEntry::getKind(){
    return string("ParameterEntry");
}

string VariableEntry::getKind(){
    return string("VariableEntry");
}

string MethodEntry::getKind(){
    return string("MethodEntry");
}

string ClassEntry::getKind(){
    return string("ClassEntry");
}

string FieldEntry::getKind(){
    return string("FieldEntry");
};

string Entry::get_name(){
    return this->name;
}

Scope::Scope(){
    this->entry_list = new vector<Entry*>();
}

ParameterEntry::ParameterEntry(const char* name, const char* parameter_type){
    this->name = string(name);
    this->parameter_type = string(parameter_type);
}

SymbolTable::SymbolTable(){
    this->current_scope = NULL;
    this->package_scope = new Scope();
    this->class_scope = new Scope();
    this->method_scope = new Scope();
}

void SymbolTable::install(Entry* entry){
    Scope* current_scope = this->get_scope();
    current_scope->install(entry);
}

void Scope::install(Entry* entry){
    if(this->lookup(entry->get_name())){
        throw string("Duplicate entry when installing entry in scope");
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

int main(){
    SymbolTable* table = new SymbolTable();
    table->open_scope();

    ParameterEntry* e = new ParameterEntry("name", "test_type");
    ParameterEntry* f = new ParameterEntry("name", "test_type");

    try{
        table->install(e);
        table->install(f);
    }
    catch(string e){
        cerr << "ERROR: " << e << endl;
        exit(1);
    }
    Entry* entry = table->lookup("name");
    cout << entry->get_name() << endl;
}
