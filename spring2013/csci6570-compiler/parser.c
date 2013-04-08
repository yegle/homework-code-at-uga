#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<cstdlib>

#include "Ast.h"
#include "y.tab.h"
#include "AstPrinter.h"
#include "SymbolTableBuilder.h"
#include "check_filename.h"

extern FILE *yyin;
extern int yydebug;

int yyparse (void);
Declaration *declTree = NULL;

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
    //yydebug = 1;
    declTree = NULL;
    try{
        yyparse();
    }
    catch(string reason){
        cerr << "ERROR: " << reason << endl;
        exit(1);
    }
    if(declTree == NULL){
        printf("declTree is NULL\n");
    }
    else{
        try{
            declTree->accept( new SymbolTableBuilder() );
        }
        catch(string reason){
            cerr << "ERROR: " << reason << endl;
            exit(1);
        }
    }
}
