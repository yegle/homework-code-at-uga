#include<stdio.h>
#include<errno.h>
#include<string.h>
#include "y.tab.h"

extern FILE *yyin;
extern int   yylineno;
extern char *yytext;

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
            default:
                printf("UNKNOWN %s\n", yytext);
        }
    }
}
