#include <string>

using namespace std;

class CodeGen: public AstVisitor {
    public:
        CodeGen();
        ~CodeGen();
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
        void ldc(const char* lexeme){
            this->write(string("ldc ") + string(lexeme));
            return;
        }
        void iload(int index){
            char buf[1024];
            snprintf(buf, 1024, "iload %d", index);
            this->write(string(buf));
            return;
        }
        void fload(int index){
            char buf[1024];
            snprintf(buf, 1024, "fload %d", index);
            this->write(string(buf));
            return;
        }
        void istore(int index){
            char buf[1024];
            snprintf(buf, 1024, "istore %d", index);
            this->write(string(buf));
            return;
        }
        void fstore(int index){
            char buf[1024];
            snprintf(buf, 1024, "fstore %d", index);
            this->write(string(buf));
            return;
        }
        void astore(int index){
            char buf[1024];
            snprintf(buf, 1024, "astore %d", index);
            this->write(string(buf));
            return;
        }
        void aload(int index){
            char buf[1024];
            snprintf(buf, 1024, "aload %d", index);
            this->write(string(buf));
            return;
        }
        void iaload(){
            this->write(string("iaload"));
        }
        void faload(){
            this->write(string("faload"));
        }
        void i2f(){
            this->write(string("i2f"));
        }
        void write(string line);
        void write(const char* line){
            this->write(string(line));
        }
        void error(int);
        void info(int, string);
        void info(int, const char*);
        void debug(int, string);
        void debug(int, const char*);
        void start_loop(){
            this->write(string("<again-label>:"));
            this->indent++;
        }
        void end_loop(){
            this->write(string("<end-label>:"));
            this->indent--;
        }
    private:
        char buf[1024];
        int indent;
};
