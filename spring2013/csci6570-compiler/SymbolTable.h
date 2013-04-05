
enum Kind {
    CLASS,
    VARIABLE,
    METHOD,
    FIELD,
    PARAMETER,
};

class Entry {
    public:
        Kind get_kind();
        string get_name();
    protected:
        Kind kind;
        string name;
};

class ParameterEntry: public Entry {
    public:
        string getKind();

        ParameterEntry(const char*, const char*);
    private:
        string parameter_type;
};

class VariableEntry: Entry {
    public:
        string getKind();
    private:
        char* variable_name;
        char* variable_type;
        char* init_value;
};

class MethodEntry: Entry {
    public:
        string getKind();
    private:
        char* method_name;
        char* return_type;
        vector<ParameterEntry *>* parameter_list;
        vector<VariableEntry *> variable_list;
};

class ClassEntry: Entry {
    public:
        string getKind();
    private:
        vector<Entry *> field_list;
        vector<MethodEntry *> method_list;
        char* class_name;
};

class FieldEntry: Entry {
    public:
        string getKind();
    private:
        char* field_name;
        char* init_value;
        char* type;
};

class Scope {
    public:
        Scope();
        ~Scope();

        void install(Entry*);
        Entry* lookup(string);
        void clear();
    private:
        vector<Entry *>* entry_list;
};

class SymbolTable {
    public:
        void open_scope();
        void install(Entry*);
        void close_scope();
        Entry* lookup(const char*);

        Scope* get_scope();

        SymbolTable();
        ~SymbolTable();
    private:
        Scope* package_scope;
        Scope* class_scope;
        Scope* method_scope;
        Scope* current_scope;
};

