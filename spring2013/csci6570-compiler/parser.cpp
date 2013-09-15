#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<cstdlib>
#include<unistd.h>

#include "Ast.h"
#include "y.tab.h"
#include "AstPrinter.h"
#include "SymbolTableBuilder.h"
#include "CodeGen.h"
#include "check_filename.h"

extern FILE *yyin;
extern int yydebug;
extern SymbolTable* table;

int yyparse (void);
Declaration *declTree = NULL;

void usage(){
    fprintf(stderr, "Usage: prog TinyJavaProgram.java\n");
    return;
}

int main(int argc, char* argv[]){
    try {
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
            SymbolTableBuilder* builder = new SymbolTableBuilder();
            declTree->accept(builder);
            //declTree->accept( new AstPrinter(0) );
            string filename = basename(argv[1]);
            string filename_wo_ext = filename.substr(0, filename.find_last_of("."));
            table->use_scope("package");
            Entry* e = table->lookup(filename_wo_ext.c_str());
            if(e == NULL){
                throw string("There should be at least one class whose name matches the filename");
            }

            if(not builder->has_error){
                declTree->accept(new CodeGen());
                //declTree->accept(new AstPrinter(0));
            }
        }
    }
    catch(string reason){
        cerr << "ERROR: " << reason << endl;
        exit(1);
    }
}
