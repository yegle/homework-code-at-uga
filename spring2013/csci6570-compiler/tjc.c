#include<stdio.h>
#include<errno.h>
#include<string.h>
#include "y.tab.h"

extern FILE *yyin;
extern int   yylineno;
extern char *yytext;
extern int commentDepth;
extern int inBlockComment;

void usage(){
    fprintf(stderr, "Usage: prog TinyJavaProgram.java\n");
    return;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        usage();
        return -1;
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
        if (token == BCOMMENT){
            if (inBlockComment){
                printf("BLOCK COMMENT BEGIN\n");
            }
            else{
                printf("BLOCK COMMENT END\n");
            }
            continue;
        }
        else if (inBlockComment){
            printf("IN BLOCK COMMENT\n");
            continue;
        }
        switch(token){
            case IDENT:
                printf("IDENT %s\n", yytext);
                break;
            case FLOAT:
                printf("FLOAT %s\n", yytext);
                break;
            case CONST:
                printf("CONST %s\n", yytext);
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
            case RETURN:
                printf("RETURN\n");
                break;
            case MINUS:
                printf("MINUS\n");
                break;
            case PLUS:
                printf("PLUS\n");
                break;
            case MUL:
                printf("MUL\n");
                break;
            case DIVIDE:
                printf("DIVIDE\n");
                break;
            default:
                printf("UNKNOWN '%s\n'", yytext);
        }
    }
}
