#include <string>

#include "Ast.h"
#include "y.tab.h"


using namespace std;

enum Kind {
    KCLASS,
    KVARIABLE,
    KMETHOD,
    KFIELD,
    KPARAMETER,
};

class Entry {
    public:
        virtual Kind get_kind();
        string get_name();
    protected:
        Kind kind;
        string name;
};

class ParameterEntry: public Entry {
    public:
        ParameterEntry(const char*, yytokentype);
        ParameterEntry(ParameterDeclaration*);
    private:
        yytokentype parameter_type;
};

class VariableEntry: public Entry {
    public:
        VariableEntry(const char*, yytokentype, string);
    private:
        yytokentype variable_type;
        string init_value;
};

class MethodEntry: public Entry {
    public:
        MethodEntry(const char*, yytokentype);
        MethodEntry(const char*, yytokentype, vector<ParameterEntry*>*, vector<VariableEntry*>*);
        void setParameters(vector<Declaration*>*);
    private:
        yytokentype return_type;
        vector<ParameterEntry *>* parameter_list;
        vector<VariableEntry *>* variable_list;
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
        FieldEntry(const char*, yytokentype, string);
    private:
        yytokentype type;
        string init_value;
};

class Scope {
    public:
        Scope();
        ~Scope();

        Scope(const char*);

        void install(Entry*);
        Entry* lookup(string);
        void clear();

        string get_name();
    private:
        string name;
        vector<Entry *>* entry_list;
};

class SymbolTable {
    public:
        void open_scope();
        void install(Entry*);
        Entry* lookup(const char*);

        void use_scope(const char*);

        Scope* get_scope();

        SymbolTable();
        ~SymbolTable();
    private:
        Scope* package_scope;
        Scope* class_scope;
        Scope* method_scope;
        Scope* current_scope;
};

