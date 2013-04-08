#include <string>
#include <map>

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
        ParameterEntry(const char*, int);
        ParameterEntry(ParameterDeclaration*);
    private:
        int parameter_type;
};

class VariableEntry: public Entry {
    public:
        VariableEntry(const char*, int, string);
    private:
        int variable_type;
        string init_value;
};

class MethodEntry: public Entry {
    public:
        MethodEntry(const char*, int);
        MethodEntry(const char*, int, vector<ParameterEntry*>*, vector<VariableEntry*>*);
        void setParameters(vector<Declaration*>*);
        void setParameters(vector<ParameterEntry *>*);
    private:
        int return_type;
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
        FieldEntry(const char*, yytokentype, const char*);
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
        void set_name(const char*);
    private:
        string name;
        vector<Entry *>* entry_list;
        map<string, Entry*>* list;
};

class SymbolTable {
    public:
        void open_scope();
        void install(Entry*);
        Entry* lookup(const char*);

        void use_scope(const char*);

        Scope* get_scope();
        Scope* get_scope(const char*);

        SymbolTable();
        ~SymbolTable();
    private:
        Scope* package_scope;
        Scope* class_scope;
        Scope* method_scope;
        Scope* simpleio_scope;
        Scope* current_scope;
};

