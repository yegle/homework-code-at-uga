#include <string>
#include <stack>

#include "SymbolTable.h"

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
        void astore(bool is_int){
            if(is_int){
                this->write(string("iastore"));
            }
            else{
                this->write(string("fastore"));
            }
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
        void f2i(){
            this->write(string("f2i"));
        }
        void ineg(){
            this->write(string("ineg"));
        }
        void fneg(){
            this->write(string("fneg"));
        }
        void neg(bool is_int){
            if(is_int){
                this->ineg();
            }
            else{
                this->fneg();
            }
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
        void store(int index, bool is_int){
            if(is_int){
                this->istore(index);
            }
            else{
                this->fstore(index);
            }
        }
        void load(int index, bool is_int){
            if(is_int){
                this->iload(index);
            }
            else{
                this->fload(index);
            }
        }
        void aload(bool is_int){
            if(is_int){
                this->write(string("iaload"));
            }
            else{
                this->write(string("faload"));
            }
        }
        void new_middle_label(){
            char buf[1024];
            this->current_index++;
            this->indexes->push(this->current_index);
            this->indent++;
        }
        string get_middle_label(){
            char buf[1024];
            snprintf(buf, 1024, "middle_label_%d", this->current_index);
            return string(buf);
        }

        void middle_label(){
            char buf[1024];
            snprintf(buf, 1024, "%s:", this->get_middle_label().c_str());
            this->write(string(buf));
        }
        void end_label(){
            char buf[1024];
            snprintf(buf, 1024, "%s:", this->get_end_label().c_str());
            this->write(string(buf));
            this->indexes->pop();
            this->indent--;
        }
        string get_end_label(){
            char buf[1024];
            int index = this->indexes->top();
            snprintf(buf, 1024, "end_label_%d", index);
            return string(buf);
        }
        void putstatic(string field_spec){
            char buf[1024];
            snprintf(buf, 1024, "putstatic %s", field_spec.c_str());
            this->write(string(buf));
        }
        void getstatic(string field_spec){
            char buf[1024];
            snprintf(buf, 1024, "getstatic %s", field_spec.c_str());
            this->write(string(buf));
        }
        void iadd(){
            this->write(string("iadd"));
        }
        void fadd(){
            this->write(string("fadd"));
        }
        void add(bool is_int){
            if(is_int){
                this->iadd();
            }
            else{
                this->fadd();
            }
        }
        void isub(){
            this->write(string("isub"));
        }
        void fsub(){
            this->write(string("fsub"));
        }
        void sub(bool is_int){
            if(is_int){
                this->isub();
            }
            else{
                this->fsub();
            }
        }
        void _goto(string label){
            char buf[1024];
            snprintf(buf, 1024, "goto %s", label.c_str());
            this->write(string(buf));
        }
        void inc_dec_ldc(bool is_int){
            if(is_int){
                this->ldc("1");
            }
            else{
                this->ldc("1.0f");
            }
        }
        void ireturn(){
            this->write(string("ireturn"));
        }
        void freturn(){
            this->write(string("freturn"));
        }
        void _return(bool is_int){
            if(is_int){
                this->ireturn();
            }
            else{
                this->freturn();
            }
        }
        void _return(){
            this->write(string("return"));
        }
        void write_operator(int op, bool is_int, bool is_array, bool is_end_label){
            char buf[1024];
            string label;
            if(is_end_label){
                label = this->get_end_label();
            }
            else{
                label = this->get_middle_label();
            }
            if(is_int){
                switch(op){
                    case AstNode::NEOP:
                        snprintf(buf, 1024,
                                "if_icmpne %s", label.c_str());
                        break;
                    case AstNode::EQOP:
                        snprintf(buf, 1024,
                                "if_icmpeq %s", label.c_str());
                        break;
                    case AstNode::GEOP:
                        snprintf(buf, 1024,
                                "if_icmpge %s", label.c_str());
                        break;
                    case AstNode::GTOP:
                        snprintf(buf, 1024,
                                "if_icmpgt %s", label.c_str());
                        break;
                    case AstNode::LTOP:
                        snprintf(buf, 1024,
                                "if_icmplt %s", label.c_str());
                        break;
                    case AstNode::LEOP:
                        snprintf(buf, 1024,
                                "if_icmple %s", label.c_str());
                        break;
                    default:
                        snprintf(this->buf, 1024, "Unrecognized OP %s!",
                                AstNode::operator2string(op));
                        this->error();
                        return;
                }
                this->write(string(buf));
            }
            else if (not is_array){
                switch(op){
                    case AstNode::NEOP:
                        snprintf(buf, 1024,
                                "ifne %s", label.c_str());
                        break;
                    case AstNode::EQOP:
                        snprintf(buf, 1024,
                                "ifeq %s", label.c_str());
                        break;
                    case AstNode::GEOP:
                        snprintf(buf, 1024,
                                "ifge %s", label.c_str());
                        break;
                    case AstNode::GTOP:
                        snprintf(buf, 1024,
                                "ifgt %s", label.c_str());
                        break;
                    case AstNode::LTOP:
                        snprintf(buf, 1024,
                                "iflt %s", label.c_str());
                        break;
                    case AstNode::LEOP:
                        snprintf(buf, 1024,
                                "ifle %s", label.c_str());
                        break;
                    default:
                        snprintf(this->buf, 1024, "Unrecognized OP %s!",
                                AstNode::operator2string(op));
                        this->error();
                        return;
                }
                this->write(string("fcmpg"));
                this->write(string(buf));
            }
            else {
                switch(op){
                    case AstNode::NEOP:
                        snprintf(buf, 1024,
                                "if_acmpne middle_label_%d",
                                this->current_index);
                        break;
                    case AstNode::EQOP:
                        snprintf(buf, 1024,
                                "if_acmpeq middle_label_%d",
                                this->current_index);
                        break;
                    default:
                        snprintf(this->buf, 1024, "Unsupported OP!");
                        this->error();
                        return;
                }
                this->write(string(buf));
            }
        }
    private:
        char buf[1024];
        int indent;
        int current_index;
        int lineno;
        stack<int>* indexes;
        MethodEntry* current_method;
        ClassEntry* current_class;
};
