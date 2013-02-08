#include<stdio.h>
#include<string.h>

extern FILE *yyin;
extern yydebug;

int main(int argc, char* argv[]){
    yydebug = 1;
    yyin = fopen("sample.java", "r");
    yyparse();
}
