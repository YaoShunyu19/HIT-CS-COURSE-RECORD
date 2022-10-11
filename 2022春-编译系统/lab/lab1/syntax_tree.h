#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int yylineno;

extern char* yytext;

void yyerror(char *msg);

typedef struct Node{
    int line;
    char* name;
    struct Node *fchild,*next;
    union{
        // id or int/float value
        char* id_type;
        int intval;
        float fltval;
    };
}* ST,* tnode;

ST newST(char* name,int num,...);

void Preorder(ST st,int level);

int nodeNum;

tnode nodeList[5000];
int nodeIsChild[5000];

void setChildTag(tnode node);

int is_wrong;
