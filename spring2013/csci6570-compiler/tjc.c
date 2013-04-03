#include<stdio.h>
#include<errno.h>
#include<string.h>

#include "Ast.h"
#include "y.tab.h"
#include "check_filename.h"


extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern char *reason;

int yylex(void);

void usage(){
    fprintf(stderr, "Usage: prog TinyJavaProgram.java\n");
    return;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        usage();
        return -1;
    }
    if (!check_filename(argv[1])){
        fprintf(stderr, "Error: the filename should end with .java\n");
        usage();
        return -2;
    }
    yyin = fopen(argv[1], "r");
    if(yyin == NULL){
        fprintf(stderr, "Error opening file. Reason: %s\n",
                strerror(errno));
        usage();
        return -2;
    }

    int token;

    while((token = yylex())!=0){
        switch(token){
            case ERROR:
                printf("ERROR on line %d: %s\n", yylineno, yytext);
                printf("REASON: %s\n", reason);
                exit(-1);
            case ASSIGN:
                printf("ASSIGN %s\n", yytext);
                break;
            case GREATEREQUAL:
                printf("GREATEREQUAL %s\n", yytext);
                break;
            case LESSEQUAL:
                printf("LESSEQUAL %s\n", yytext);
                break;
            case KEYWORD:
                printf("KEYWORD %s\n", yytext);
                break;
            case IDENT:
                printf("IDENT %s\n", yytext);
                break;
            case FLOATLITERAL:
                printf("FLOATLITERAL %s\n", yytext);
                break;
            case INTLITERAL:
                printf("INTLITERAL %s\n", yytext);
                break;
            case STRING:
                printf("STRING %s\n", yytext);
                break;
            case LPAR:
                printf("LPAR %s\n", yytext);
                break;
            case RPAR:
                printf("RPAR %s\n", yytext);
                break;
            case LBRACE:
                printf("LBRACE %s\n", yytext);
                break;
            case RBRACE:
                printf("RBRACE %s\n", yytext);
                break;
            case LBRACKET:
                printf("LBRACKET %s\n", yytext);
                break;
            case RBRACKET:
                printf("RBRACKET %s\n", yytext);
                break;
            case SEMI:
                printf("SEMI\n");
                break;
            case EQUAL:
                printf("EQUAL\n");
                break;
            case LESS:
                printf("LESS\n");
                break;
            case GREATER:
                printf("GREATER\n");
                break;
            case DQUOTE:
                printf("DQUOTE\n");
                break;
            case DOT:
                printf("DOT\n");
                break;
            case ICOMMENT:
                printf("ICOMMENT\n");
                break;
            case BCOMMENTSTART:
                printf("BCOMMENTSTART\n");
                break;
            case BCOMMENTEND:
                printf("BCOMMENTEND\n");
                break;
            case RETURN:
                printf("RETURN\n");
                break;
            case MINUS:
                printf("MINUS\n");
                break;
            case PLUS:
                printf("PLUS\n");
                break;
            case INCREMENT:
                printf("INCREMENT\n");
                break;
            case DECREMENT:
                printf("DECREMENT\n");
                break;
            case MUL:
                printf("MUL\n");
                break;
            case DIVIDE:
                printf("DIVIDE\n");
                break;
            case COMMA:
                printf("COMMA\n");
                break;
            case NOTEQUAL:
                printf("NOTEQUAL\n");
                break;
            default:
                printf("UNKNOWN '%s\n'", yytext);
        }
    }
}
