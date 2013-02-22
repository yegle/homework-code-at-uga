/* 
 * token declarations
 */

%{

#include <stdio.h>
#include <errno.h>
#include "y.tab.h"

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
int yyerrstatus;
int yydebug=1;

%}

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

%%

tiny_java_program: class_decl
                 ;

class_decl: class_decl PUBLIC CLASS IDENT LBRACE member_decl_list RBRACE
          | empty
          ;

member_decl_list: member_decl
                |
                member_decl member_decl_list
                ;

member_decl: field_decl
           |
           method_decl
           ;

field_decl: STATIC type IDENT ASSIGN literal SEMI
          |
          STATIC type IDENT ASSIGN literal err {
            error_semi();
          }
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index SEMI
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index err {
            error_semi();
          }
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index list_literal SEMI
          |
          STATIC type LBRACKET RBRACKET IDENT ASSIGN NEW type array_define_index list_literal err {
            error_semi();
          }
          ;

array_define_index: array_index
                  |
                  LBRACKET RBRACKET
                  ;


method_decl: STATIC type IDENT LPAR formal_param_list RPAR LBRACE method_body RBRACE
           |
           STATIC VOID IDENT LPAR formal_param_list RPAR LBRACE method_body RBRACE
           |
           STATIC type IDENT LPAR RPAR LBRACE method_body RBRACE
           |
           STATIC VOID IDENT LPAR RPAR LBRACE method_body RBRACE
           |
           PUBLIC STATIC VOID IDENT LPAR IDENT LBRACKET RBRACKET IDENT RPAR LBRACE method_body RBRACE
           ;

array_index: LBRACKET INTLITERAL RBRACKET
           |
           LBRACKET qualified_name RBRACKET
           ;

type: INT
    |
    FLOAT
    |
    INT LBRACKET RBRACKET
    |
    FLOAT LBRACKET RBRACKET
    ;


formal_param_list: formal_param
                 |
                 formal_param COMMA formal_param_list
                 ;

formal_param: type IDENT
            ;

method_body: local_decl_list method_statement_list
           ;

local_decl_list: local_decl local_decl_list
               | empty
               ;

local_decl: type IDENT ASSIGN literal SEMI
          |
          type IDENT ASSIGN literal {
            error_semi();
          }
          |
          type IDENT ASSIGN expression SEMI
          |
          type IDENT ASSIGN expression err {
            error_semi();
          }
          |
          type IDENT array_index ASSIGN literal SEMI
          |
          type IDENT array_index ASSIGN literal {
            error_semi();
          }
          ;

method_statement_list: statement method_statement_list
                     |
                     return_statement
                     ;

statement_list: statement statement_list
              | empty
              ;

statement: IDENT ASSIGN expression SEMI
         |
         IDENT ASSIGN expression err {
            error_semi();
         }
         |
         IDENT ASSIGN NEW type expression SEMI
         |
         IDENT ASSIGN NEW type expression err {
            error_semi();
         }
         |
         IDENT array_index ASSIGN expression SEMI
         |
         IDENT array_index ASSIGN expression err {
            error_semi();
         }
         |
         IDENT array_index ASSIGN expression list_literal SEMI
         |
         IDENT array_index ASSIGN expression list_literal err {
            error_semi();
         }
         |
         IF LPAR expression RPAR statement
         |
         IF LPAR expression RPAR statement ELSE statement
         |
         WHILE LPAR expression RPAR statement
         |
         FOR LPAR for_part_1 for_part_2 for_part_3 RPAR statement
         |
         method_invocation SEMI
         |
         method_invocation err {
            error_semi();
         }
         |
         LBRACE statement_list RBRACE
         |
         expression SEMI
         |
         expression err {
            error_semi();
         }
         |
         SEMI
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
                |
                RETURN expression err {
                    error_semi();
                }
                |
                RETURN SEMI
                |
                RETURN err {
                    error_semi();
                }
                ;

method_invocation: qualified_name LPAR argument_list RPAR
                 |
                 qualified_name LPAR RPAR
                 ;

qualified_name: IDENT DOT IDENT
              |
              IDENT
              ;

argument_list: expression
             |
             expression COMMA argument_list
             ;

expression: relational_expression
          |
          relational_expression EQUAL relational_expression
          |
          relational_expression NOTEQUAL relational_expression
          ;

relational_expression: additive_expression
                     |
                     additive_expression GREATER additive_expression
                     |
                     additive_expression LESS additive_expression
                     |
                     additive_expression GREATEREQUAL additive_expression
                     |
                     additive_expression LESSEQUAL additive_expression
                     ;

additive_expression: multiplicative_expression
                   |
                   additive_expression PLUS multiplicative_expression
                   |
                   additive_expression MINUS multiplicative_expression
                   ;

multiplicative_expression: unary_expression
                         |
                         multiplicative_expression MUL unary_expression
                         |
                         multiplicative_expression DIVIDE unary_expression
                         ;

unary_expression: primary_expression
                |
                PLUS unary_expression
                |
                MINUS unary_expression
                |
                LPAR type RPAR unary_expression
                ;

primary_expression: literal
                  |
                  IDENT
                  |
                  method_invocation
                  |
                  LPAR expression RPAR
                  |
                  inc_dec_operator IDENT
                  |
                  IDENT inc_dec_operator
                  |
                  IDENT array_index
                  |
                  NEW INT array_index
                  |
                  NEW FLOAT array_index
                  ;

literal: INTLITERAL
       |
       FLOATLITERAL
       |
       STRING
       |
       list_literal
       |
       NULL_
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
