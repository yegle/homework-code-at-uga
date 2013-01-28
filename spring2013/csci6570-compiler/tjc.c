#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdbool.h>
#include "y.tab.h"


extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern char *reason;

void usage(){
    fprintf(stderr, "Usage: prog TinyJavaProgram.java\n");
    return;
}

_Bool check_filename(const char* filename){
    int check_len = sizeof(".java");
    char *str = ".java";

    int str_len = strlen(str);
    int filename_len = strlen(filename);

    if (filename_len < check_len){
        return false;
    }

    bool ret = true;
    for (int i=0; i<check_len; i++){
        if(str[str_len-i] != filename[filename_len-i]){
            ret = false;
            break;
        }
    }
    return ret;
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
            case KEYWORD:
                printf("KEYWORD %s\n", yytext);
                break;
            case IDENT:
                printf("IDENT %s\n", yytext);
                break;
            case FLOAT:
                printf("FLOAT %s\n", yytext);
                break;
            case INTEGER:
                printf("INTEGER %s\n", yytext);
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
