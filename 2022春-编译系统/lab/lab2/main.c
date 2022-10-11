#include<stdio.h>
#include<stdlib.h>
#include"semantic.h"

extern FILE* yyin;
extern int yylineno;
extern int yyparse();
extern int yyrestart(FILE*);
extern struct Node* head;
extern int is_error;
int yylex();



int main(int argc,char** argv){
    if(argc<=1)
        return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yylineno=1;
    yyrestart(f);
    yyparse();
    if(is_error==0){
        //print_tree(head);
        Program(head);
    }
    return 0;
}

