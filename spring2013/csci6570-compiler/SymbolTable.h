#include <string>
#include <map>

#include "Ast.h"
#include "y.tab.h"


using namespace std;


class Entry {
    public:
        AstNode::AstNodeKind get_kind();
        string get_name();
    protected:
        AstNode::AstNodeKind kind;
        string name;
};

class ParameterEntry: public Entry {
    public:
        ParameterEntry(const char*, int);
        ParameterEntry(ParameterDeclaration*);
        int get_parameter_type();
        int parameter_base_type();
        int get_index() {
            return this->index;
        }
        void set_index(int index){
            this->index = index;
            return;
        }
    private:
        int parameter_type;
        int index;
};

class VariableEntry: public Entry {
    public:
        VariableEntry(const char*, int, LiteralExpression*);
        VariableEntry(VariableDeclaration*);
        int get_variable_type();
        int variable_base_type();
        int get_index(){
            return this->index;
        }
        void set_index(int index){
            this->index = index;
            return;
        }
    private:
        int variable_type;
        LiteralExpression* init_expression;
        int index;
};

class MethodEntry: public Entry {
    public:
        MethodEntry(const char*, const char*, int);
        MethodEntry(const char*, const char*, int, vector<ParameterEntry*>*, vector<VariableEntry*>*);
        void setParameters(vector<Declaration*>*);
        void setParameters(vector<ParameterEntry *>*);
        vector<ParameterEntry*>* getParameters();

        void setVariables(vector<Declaration*>*);
        void setVariables(vector<VariableEntry*>*);
        vector<VariableEntry*>* getVariables();
        int get_current_index(){
            return this->current_index;
        }
        void inc_current_index(){
            this->current_index += 1;
        }
        int get_return_type(){
            return this->return_type;
        }
        string get_return_type_spec(){
            return this->return_type_spec;
        }
        string get_arg_specs(){
            return this->arg_specs;
        }
    private:
        int return_type;
        vector<ParameterEntry *>* parameter_list;
        vector<VariableEntry *>* variable_list;
        int current_index;
        string return_type_spec;
        string arg_specs;
        string class_name;
};

class ClassEntry: public Entry {
    public:
        ClassEntry(const char*);
    private:
        vector<Entry *> field_list;
        vector<MethodEntry *> method_list;
};

class FieldEntry: public Entry {
    public:
        FieldEntry(const char*, int, const char*);
        int get_field_type();
        int field_base_type();
    private:
        int field_type;
        string init_value;
};

class Scope {
    public:
        //Scope();
        ~Scope();

        Scope(const char*);

        void install(Entry*);
        Entry* lookup(string);
        void clear();
        void print();

        string get_name();
        void set_name(const char*);
    private:
        string name;
        vector<Entry *>* entry_list;
        map<string, Entry*>* list;
};

class SymbolTable {
    public:
        //void open_scope();
        void install(Entry*);
        Entry* lookup(const char*);

        void use_scope(const char*);

        Scope* get_scope();
        Scope* get_scope(const char*);

        SymbolTable();
        ~SymbolTable();
        string get_current_method_name(){
            return this->current_method_name;
        }
    private:
        //Scope* package_scope;
        //Scope* class_scope;
        //Scope* method_scope;
        //Scope* simpleio_scope;
        Scope* current_scope;
        map<string, Scope*>* scopes;
        string current_method_name;
};
