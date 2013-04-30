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
        void error();
        void info(string);
        void info(const char*);
        void debug(string);
        void debug(const char*);
        void start_loop(){
            char buf[1024];
            snprintf(buf, 1024, "loop_start_%d:", this->current_index);
            this->write(string(buf));
            this->current_index++;
            this->indent++;
        }
        void end_loop(){
            snprintf(buf, 1024, "loop_end_%d:", this->current_index);
            this->write(string(buf));
            this->current_index++;
            this->indent--;
        }

        void gen_new_index(){
            this->current_index++;
        }
        void write_operator(int op, bool is_int){
            char buf[1024];
            if(is_int){
                switch(op){
                    case AstNode::NEOP:
                        snprintf(buf, 1024,
                                "if_icmpne end_label_%d",
                                this->current_index);
                        break;
                    case AstNode::EQOP:
                        snprintf(buf, 1024,
                                "if_icmpeq end_label_%d",
                                this->current_index);
                        break;
                    case AstNode::GEOP:
                        snprintf(buf, 1024,
                                "if_icmpge end_label_%d",
                                this->current_index);
                        break;
                    default:
                        snprintf(this->buf, 1024, "Unrecognized OP!");
                        this->error();
                        return;
                }
                this->write(string(buf));
            }
            else{
                switch(op){
                    case AstNode::NEOP:
                        snprintf(buf, 1024,
                                "ifne end_label_%d",
                                this->current_index);
                        break;
                    case AstNode::EQOP:
                        snprintf(buf, 1024,
                                "ifeq end_label_%d",
                                this->current_index);
                        break;
                    case AstNode::GEOP:
                        snprintf(buf, 1024,
                                "ifge end_label_%d",
                                this->current_index);
                        break;
                    default:
                        snprintf(this->buf, 1024, "Unrecognized OP!");
                        this->error();
                        return;
                }
                this->write(string("fcmpg"));
                this->write(string(buf));
            }
        }
    private:
        char buf[1024];
        int indent;
        int current_index;
        int lineno;
};
