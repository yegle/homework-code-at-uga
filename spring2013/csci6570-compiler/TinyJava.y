/* 
 * token declarations
 */

%{

#include <stdio.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include "string.h"
#include "Ast.h"
#include "y.tab.h"

#include "SymbolTable.h"

using namespace std;

#define YYERROR_VERBOSE

void free(void *ptr);
void yyerror( const char *msg );
void error( const char *msg );
int yylex( void );
void error_semi();
void clear_error();

extern int yylineno;
extern int yyin;
extern char* yytext;
extern int yychar;
int yyerrstatus;
extern int yydebug;
extern Declaration *declTree;

MethodDeclaration *methodDecl = NULL;
ClassDeclaration *classDecl = NULL;
SymbolTable* table = new SymbolTable();
%}

%union {
  int	          	 ival;    //e.g., type representation
  char           	*sval;    //e.g., lexemes (idents, literals)
  Entry*          	 eval;    //symbol table entry
  Statement      	*tval;    //a Statement object reference
  BlockStatement 	*bval;    //a BlockStatement reference
  Expression     	*xval;    //an Expression reference
  MethodCallExpression  *mval;    //a MethodCallExpression reference
  Declaration           *dval;    //a Declaration reference
  std::vector<int>    	*vival;     //a reference to a vector of type representations
  std::vector<Expression *>  *aval;  //a reference to a vector of Expression references
                                //        (actual arguments of a method call)
  std::vector<Declaration *>  *vdval; //  a reference to a vector of Declaration references
                                //        (a list of local declarations
                                //         or formal paramater declarations)
}

%token IDENT
%token FLOATLITERAL
%token INTLITERAL
%token STRING
%token ICOMMENT
%token LPAR
%token RPAR
%token SEMI
%token DOT
%token LBRACE
%token RBRACE
%token EQUAL
%token DQUOTE
%token LESS
%token GREATER
%token LBRACKET
%token RBRACKET
%token MINUS
%token PLUS
%token MUL
%token DIVIDE
%token BCOMMENTSTART
%token BCOMMENTEND
%token ERROR
%token KEYWORD
%token ASSIGN
%token GREATEREQUAL
%token LESSEQUAL
%token INCREMENT
%token DECREMENT
%token COMMA
%token NOTEQUAL
%token RETURN
%token IF
%token ELSE
%token FOR
%token NEW
%token NULL_
%token INT
%token FLOAT
%token VOID
%token PUBLIC
%token STATIC
%token WHILE
%token CLASS

%type <sval>   INTLITERAL
%type <sval>   FLOATLITERAL
%type <sval>   STRING
%type <sval>   NULL_
%type <sval>   literal
%type <ival>   type
%type <xval>   primary_expression
%type <xval>   unary_expression
%type <xval>   multiplicative_expression
%type <xval>   additive_expression
%type <xval>   relational_expression
%type <xval>   expression
%type <tval>   statement
%type <tval>   return_statement
%type <tval>   method_body
%type <xval>   method_invocation
%type <bval>   statement_list
%type <bval>   method_statement_list
%type <aval>   argument_list
%type <dval>   local_decl
%type <vdval>  local_decl_list
%type <dval>   formal_param
%type <vdval>  formal_param_list
%type <sval>   IDENT
%type <xval>   class_decl
%type <dval>   member_decl
%type <vdval>  member_decl_list
%type <dval>   method_decl
%type <dval>   field_decl
%type <tval>   opt_else

%%

tiny_java_program: class_decl
                 {
                    declTree = classDecl;
                 }
                 ;

class_decl: class_decl PUBLIC CLASS IDENT
          {
            classDecl = new ClassDeclaration( yylineno, $4 );
          }
          LBRACE member_decl_list RBRACE
          | empty
          ;

member_decl_list: member_decl
                {
                    $$ = new vector<Declaration *>();
                    $$->push_back($1);
                }
                |
                member_decl member_decl_list
                {
                    $$ = $2;
                    $$->push_back($1);
                }
                ;

member_decl: field_decl
           {
            $$ = $1;
           }
           |
           method_decl
           {
            $$ = $1;
           }
           ;

field_decl: STATIC type IDENT ASSIGN literal SEMI
          {
            classDecl->addMember(new FieldDeclaration(yylineno, $3, $2, $5));
          }
          /*
          |
          STATIC INT IDENT ASSIGN literal SEMI
          {
	        classDecl->addMember( new FieldDeclaration( yylineno, $3, AstNode::TINT, $5 ) );
          }
          |
          STATIC FLOAT LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index SEMI
          |
          STATIC INT LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index SEMI
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index list_literal SEMI
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index list_literal err {
            error_semi();
          }
          */
          ;

array_define_index: array_index
                  |
                  LBRACKET RBRACKET
                  ;

method_decl:
           STATIC VOID IDENT LPAR
           {
              methodDecl = new MethodDeclaration( yylineno, $3, AstNode::TVOID );
              classDecl->addMember( methodDecl );


              table->open_scope();
              MethodEntry* m = new MethodEntry($3, VOID);
              table->install(m);
              methodDecl->setEntry(m);
           }
           formal_param_list RPAR
           {
            methodDecl->setParameters( $6 );
           }
           LBRACE method_body RBRACE
           |
           STATIC INT IDENT LPAR
           {
              methodDecl = new MethodDeclaration( yylineno, $3, AstNode::TINT );
              classDecl->addMember( methodDecl );
           }
           formal_param_list RPAR
           {
            methodDecl->setParameters( $6 );
           }
           LBRACE method_body RBRACE
           |
           STATIC FLOAT IDENT LPAR
           {
              methodDecl = new MethodDeclaration( yylineno, $3, AstNode::TFLOAT );
              classDecl->addMember( methodDecl );
           }
           formal_param_list RPAR
           {
            methodDecl->setParameters( $6 );
           }
           LBRACE method_body RBRACE
           |
           STATIC VOID IDENT LPAR RPAR
           {
            methodDecl = new MethodDeclaration(yylineno, $3, AstNode::TVOID);
            classDecl->addMember(methodDecl);
           }
           LBRACE method_body RBRACE
           |
           STATIC INT IDENT LPAR RPAR
           {
            methodDecl = new MethodDeclaration(yylineno, $3, AstNode::TINT);
            classDecl->addMember(methodDecl);
           }
           LBRACE method_body RBRACE
           |
           STATIC FLOAT IDENT LPAR RPAR
           {
            methodDecl = new MethodDeclaration(yylineno, $3, AstNode::TFLOAT);
            classDecl->addMember(methodDecl);
           }
           LBRACE method_body RBRACE
           |
           PUBLIC STATIC VOID IDENT LPAR IDENT LBRACKET RBRACKET IDENT RPAR
           {
              ParameterDeclaration *pd = NULL;
              if( strcmp( $6, "String" ) == 0 ) {
                pd = new ParameterDeclaration( yylineno, $9, AstNode::TSTRINGA );
              }
              else {
                pd = new ParameterDeclaration( yylineno, $9, 0 );
              }
              vector<Declaration*> *pv = new vector<Declaration*>();
              pv->push_back( pd );
              methodDecl = new MethodDeclaration( yylineno, $4, AstNode::TVOID );
              methodDecl->setParameters( pv );
              methodDecl->setPublicMethod( true );
              classDecl->addMember(methodDecl);
              // SymbolTable
              ParameterEntry* e = new ParameterEntry($9, INT);
              table->open_scope();
              table->install(e);
           }
           LBRACE method_body RBRACE
           ;

array_index: LBRACKET INTLITERAL RBRACKET
           |
           LBRACKET qualified_name RBRACKET
           ;

type: INT
    {
        $$ = AstNode::TINT;
    }
    |
    FLOAT
    {
        $$ = AstNode::TFLOAT;
    }
    ;


formal_param_list: formal_param
                 {
                 $$ = new vector<Declaration *>();
                 $$->push_back($1);
                 }
                 |
                 formal_param COMMA formal_param_list
                 {
                 $$ = $3;
                 $$->insert($$->begin(), $1);
                 }
                 ;

formal_param: type IDENT
            {
                $$ = new ParameterDeclaration(yylineno, $2, $1);
            }
            |
            type LBRACKET RBRACKET IDENT
            {
                $$ = new ParameterDeclaration(yylineno, $4, $1);
            }
            ;

method_body: local_decl_list method_statement_list
           {
            methodDecl->setVariables( $1 );
            methodDecl->setBody( $2 );
           }
           ;

local_decl_list: local_decl local_decl_list
               {
                $$ = $2;
                $$->push_back($1);
               }
               | empty
               {
	            $$ = new vector<Declaration*>();
               }
               ;

local_decl: type IDENT ASSIGN literal SEMI
          {
            $$ = new VariableDeclaration( yylineno, $2, $1, $4);
            //VariableEntry* v = new VariableEntry($2, $1, $4);
            //table->install(v);
          }
          |
          type LBRACKET RBRACKET IDENT ASSIGN literal SEMI
          {
            $$ = new VariableDeclaration( yylineno, $4, $1, $6);
          }
          /*
          |
          type IDENT array_index ASSIGN literal SEMI
          */
          ;

method_statement_list: statement method_statement_list
                     {
                        $$ = $2;
                        $$->prependStatement($1);
                     }
                     |
                     return_statement
                     {
                        $$ = new BlockStatement(yylineno);
                        $$->prependStatement($1);
                     }
                     ;

statement_list: statement statement_list
              {
                $$ = $2;
                $$->prependStatement($1);
              }
              | empty
              {
	            $$ = new BlockStatement( yylineno );
              }
              ;

opt_else: ELSE statement
        {
        $$ = new BlockStatement(yylineno);
        }
        |
        empty
        {
        $$ = new EmptyStatement(yylineno);
        }
        ;


statement: IDENT ASSIGN expression SEMI
         {
	        $$ = new AssignStatement( yylineno, $1, $3 );
         }
         |
         IDENT LBRACKET primary_expression RBRACKET ASSIGN expression SEMI
         {
	        $$ = new AssignStatement( yylineno, $1, $3, $6);
         }
         |
         IF LPAR expression RPAR statement opt_else
         {
	        $$ = new IfStatement( yylineno, $3, $5, $6 );
         }
         |
         WHILE LPAR expression RPAR statement
         {
	        $$ = new WhileStatement( yylineno, $3, $5 );
         }
         |
         FOR LPAR IDENT ASSIGN expression SEMI expression SEMI expression RPAR statement
         {
            $$ = new ForStatement(yylineno, $3, $5, $7, $9, $11);
         }
         |
         FOR LPAR IDENT LBRACKET expression RBRACKET ASSIGN expression SEMI expression SEMI expression RPAR statement
         {
            $$ = new ForStatement(yylineno, $3, $5, $8, $10, $12, $14);
         }
         |
         method_invocation SEMI
         {
	        $$ = new MethodCallStatement( yylineno, $1 );
         }
         |
         LBRACE statement_list RBRACE
         {
	        $$ = $2;
         }
         |
         expression SEMI
         |
         SEMI
         {
	        $$ = new EmptyStatement( yylineno );
         }
         ;

for_part_1: expression SEMI
          |
          local_decl
          |
          IDENT ASSIGN expression SEMI
          |
          SEMI
          ;

for_part_2: expression SEMI
          |
          SEMI
          ;

for_part_3: expression
          |
          empty
          ;

inc_dec_operator: INCREMENT
                |
                DECREMENT
                ;

return_statement: RETURN expression SEMI
                {
	                $$ = new ReturnStatement( yylineno, NULL, $2 );
                }
                |
                RETURN SEMI
                {
	                $$ = new ReturnStatement( yylineno, NULL, NULL );
                }
                ;

method_invocation: IDENT LPAR argument_list RPAR
                 {
	                $$ = new MethodCallExpression( yylineno, NULL, $1, $3 );
                 }
                 |
                 IDENT LPAR RPAR
                 {
	                $$ = new MethodCallExpression( yylineno, NULL, $1, (vector<Expression *> *) NULL );
                 }
                 |
                 IDENT DOT IDENT LPAR argument_list RPAR
                 {
	                $$ = new MethodCallExpression( yylineno, $1, $3, $5 );
                 }
                 |
                 IDENT DOT IDENT LPAR RPAR
                 {
	                $$ = new MethodCallExpression( yylineno, $1, $3, (vector<Expression *> *) NULL );
                 }
                 ;

qualified_name: IDENT DOT IDENT
              |
              IDENT
              ;


argument_list: expression
             {
              $$ = new vector<Expression *>();
              $$->push_back( $1 );
             }
             |
             argument_list COMMA expression
             {
              $$ = $1;
              $$->push_back( $3 );
             }
             ;

expression: relational_expression
          {
            $$ = $1;
          }
          |
          relational_expression EQUAL relational_expression
          {
              $$ = new BinaryExpression( yylineno, AstNode::EQOP, $1, $3 );
          }
          |
          relational_expression NOTEQUAL relational_expression
          {
              $$ = new BinaryExpression( yylineno, AstNode::NEOP, $1, $3 );
          }
          ;

relational_expression: additive_expression
                     {
                     $$ = $1;
                     }
                     |
                     additive_expression GREATER additive_expression
                     {
                          $$ = new BinaryExpression( yylineno, AstNode::GTOP, $1, $3 );
                     }
                     |
                     additive_expression LESS additive_expression
                     {
                          $$ = new BinaryExpression( yylineno, AstNode::LTOP, $1, $3 );
                     }
                     |
                     additive_expression GREATEREQUAL additive_expression
                     {
                          $$ = new BinaryExpression( yylineno, AstNode::GEOP, $1, $3 );
                     }
                     |
                     additive_expression LESSEQUAL additive_expression
                     {
                          $$ = new BinaryExpression( yylineno, AstNode::LEOP, $1, $3 );
                     }
                     ;

additive_expression: multiplicative_expression
                   {
                   $$ = $1;
                   }
                   |
                   additive_expression PLUS multiplicative_expression
                      {
                          $$ = new BinaryExpression( yylineno, AstNode::ADDOP, $1, $3 );
                      }
                   |
                   additive_expression MINUS multiplicative_expression
                      {
                          $$ = new BinaryExpression( yylineno, AstNode::SUBOP, $1, $3 );
                      }
                   ;

multiplicative_expression: unary_expression
                         {
                         $$ = $1;
                         }
                         |
                         multiplicative_expression MUL unary_expression
                         {
                            $$ = new BinaryExpression( yylineno, AstNode::MULOP, $1, $3 );
                         }
                         |
                         multiplicative_expression DIVIDE unary_expression
                         {
                            $$ = new BinaryExpression( yylineno, AstNode::DIVOP, $1, $3 );
                         }
                         ;

unary_expression: primary_expression
                {
                $$ = $1;
                }
                |
                PLUS unary_expression
                {
                    $$ = new UnaryExpression(yylineno, AstNode::ADDOP, $2);
                }
                |
                unary_expression INCREMENT
                {
                    $$ = new UnaryExpression(yylineno, AstNode::INCOP, $1, true);
                }
                |
                MINUS unary_expression
                {
                    $$ = new UnaryExpression(yylineno, AstNode::SUBOP, $2);
                }
                |
                unary_expression DECREMENT
                {
                    $$ = new UnaryExpression(yylineno, AstNode::DECOP, $1, true);
                }
                |
                LPAR type RPAR unary_expression
                {
                    $$ = new CastExpression(yylineno, $2, $4);
                }
                ;

primary_expression: INTLITERAL
                   {
                   $$ = new LiteralExpression(yylineno, $1, AstNode::TINT);
                   }
                   |
                   FLOATLITERAL
                   {
                   $$ = new LiteralExpression(yylineno, $1, AstNode::TFLOAT);
                   }
                   |
                   STRING
                   {
                   $$ = new LiteralExpression(yylineno, $1, AstNode::TSTRING);
                   }
                   |
                   NULL_
                   {
                   $$ = new LiteralExpression(yylineno, $1, AstNode::TREF);
                   }
                  |
                  IDENT
                  {
                  $$ = new ReferenceExpression(yylineno, $1);
                  }
                  |
                  IDENT LBRACKET primary_expression RBRACKET
                  {
                    $$ = new ReferenceExpression(yylineno, $1, $3);
                  }
                  |
                  method_invocation
                  {
                  $$ = $1;
                  }
                  |
                  LPAR expression RPAR
                  {
                  $$ = $2;
                  }
                  |
                  NEW INT LBRACKET primary_expression RBRACKET
                  {
                  $$ = new NewExpression(yylineno, AstNode::TINT, $4);
                  }
                  |
                  NEW FLOAT LBRACKET primary_expression RBRACKET
                  {
                  $$ = new NewExpression(yylineno, AstNode::TFLOAT, $4);
                  }
                  ;

literal: INTLITERAL
       {
       $$ = $1;
       }
       |
       FLOATLITERAL
       {
       $$ = $1;
       }
       |
       STRING
       {
       $$ = $1;
       }
       |
       NULL_
       {
       $$ = $1;
       }
       ;

list_literal: LBRACE int_list RBRACE
            |
            LBRACE float_list RBRACE
            |
            LBRACE string_list RBRACE
            |
            LBRACE RBRACE
            ;

int_list: INTLITERAL
        |
        INTLITERAL COMMA int_list
        ;

float_list: FLOATLITERAL
          |
          FLOATLITERAL COMMA float_list
          ;

string_list: STRING
           |
           STRING COMMA string_list
           ;

empty: ;

err: ;
%%


void
yyerror( const char *msg )
{
  printf("Line %d at \"%s\": %s\n", yylineno, yytext, msg);
  yyclearin;
  yyerrok;
}

void error(const char *msg){
    yyerror(msg);
}

void error_semi(){
    error("Expecting a SEMI here");
}

void clear_error(){
    yyclearin;
    yyerrok;
}
