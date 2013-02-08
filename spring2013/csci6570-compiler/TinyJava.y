/* 
 * token declarations
 */

%{

#include <stdio.h>

void free(void *ptr);
void yyerror( const char *msg );
int yylex( void );

extern int yylineno;

%}

%token IDENT
%token FLOAT
%token INTEGER
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
%token RETURN
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

%%

tiny_java_program: class_decl
                 ;

class_decl: 'public' 'class' IDENT LBRACE member_decl_list RBRACE
          ;

member_decl_list: member_decl
                |
                member_decl member_decl
                ;

member_decl: field_decl
           |
           method_decl
           ;

field_decl: 'static' type IDENT ASSIGN literal SEMI
          ;

method_decl: 'static' return_type IDENT LPAR formal_param_list RPAR LBRACE method_body RBRACE
           |
           'static' return_type IDENT LPAR RPAR LBRACE method_body RBRACE
           |
           'public' 'static' 'void' IDENT LPAR IDENT LBRACKET RBRACKET IDENT RPAR LBRACE method_body RBRACE
           ;

type: 'int'
    |
    'float'
    ;

return_type: type
           |
           'void'
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
         'if' LPAR expression RPAR statement
         |
         'if' LPAR expression RPAR statement 'else' statement
         |
         'while' LPAR expression RPAR statement
         |
         method_invocation SEMI
         |
         LBRACE statement_list RBRACE
         |
         SEMI
         ;

return_statement: 'return' expression SEMI
                |
                'return' SEMI
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
                LPAR type RPAR unary_expression
                ;

primary_expression: literal
                  |
                  IDENT
                  |
                  method_invocation
                  |
                  LPAR expression RPAR
                  ;

literal: INTEGER
       |
       FLOAT
       |
       STRING
       ;

empty: ;
%%


void
yyerror( const char *msg )
{
  printf("Line %d: %s\n", yylineno, msg);
}


