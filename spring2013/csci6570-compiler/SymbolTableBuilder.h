class SymbolTableBuilder: public AstVisitor {
    public:
        SymbolTableBuilder();
        ~SymbolTableBuilder();
        void visit( MethodDeclaration *aDeclNode );
        void visit( FieldDeclaration *aDeclNode );
        void visit( ParameterDeclaration *aDeclNode );
        void visit( VariableDeclaration *aDeclNode );
        void visit( ClassDeclaration *aDeclNode );
        void visit( LiteralExpression *anExpNode );
        void visit( ReferenceExpression *anExpNode );
        void visit( NewExpression *anExpNode );
        void visit( UnaryExpression *anExpNode );
        void visit( BinaryExpression *anExpNode );
        void visit( CastExpression *anExpNode );
        void visit( MethodCallExpression *anExpNode );
        void visit( AssignStatement *anStmtNode );
        void visit( WhileStatement *anStmtNode );
        void visit( ForStatement *anStmtNode );
        void visit( IfStatement *anStmtNode );
        void visit( ReturnStatement *anStmtNode );
        void visit( BlockStatement *anStmtNode );
        void visit( MethodCallStatement *anStmtNode );
        void visit( EmptyStatement *anStmtNode );
        void error(int);
        void info(int, string);
        void info(int, const char*);
        void debug(int, string);
        void debug(int, const char*);
    private:
        ClassEntry* current_class;
        MethodEntry* current_method;
        char buf[1024];
};
