%{
#include<stdio.h>
#include"syntax.tab.h"
int yycolumn=1;
extern int is_error;
#define YY_USER_ACTION\
    yylloc.first_line=yylloc.last_line=yylineno;\
    yylloc.first_column=yycolumn;\
    yylloc.last_column=yycolumn+yyleng-1;\
    yycolumn+=yyleng;
%}
%option yylineno
IGNORE [\r \t]
CHANGELINE [\n]
INT 0|[1-9][0-9]*
FLOAT {INT}\.[0-9]+
ID [_a-zA-Z][0-9_a-zA-Z]*
TYPE int|float
%%
"//" {
    char c=input();
    while(c!='\n') c=input();
    }
"/*" {
    char a=input();
    while(a!=EOF) 
        {
            if(a=='*')
            {
                a=input();
                if(a=='/')
                {
                    break;
                }
            }
            else a=input();
        }
    if(a==EOF) printf("Error type B at Line %d: LACK OF */.\n",yylineno);
    }
"struct" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"STRUCT\0");
    newnode->judge=0;
    yylval=newnode;
    return STRUCT;
    }
"return" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"RETURN\0");
    newnode->judge=0;
    yylval=newnode;
    return RETURN;
    }
"if" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"IF\0");
    newnode->judge=0;
    yylval=newnode;
    return IF;
    }
"else" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"ELSE\0");
    newnode->judge=0;
    yylval=newnode;
    return ELSE;
    }
"while" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"WHILE\0");
    newnode->judge=0;
    yylval=newnode;
    return WHILE;
    }
";" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"SEMI\0");
    newnode->judge=0;
    yylval=newnode;
    return SEMI;
    }
"." {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"DOT\0");
    newnode->judge=0;
    yylval=newnode;
    return DOT;
    }
"," {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"COMMA\0");
    newnode->judge=0;
    yylval=newnode;
    return COMMA;
    }
"=" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"ASSIGNOP\0");
    newnode->judge=0;
    yylval=newnode;
    return ASSIGNOP;
    }
"+" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"PLUS\0");
    newnode->judge=0;
    yylval=newnode;
    return PLUS;
    }
"-" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"MINUS\0");
    newnode->judge=0;
    yylval=newnode;
    return MINUS;
    }
"*" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"STAR\0");
    newnode->judge=0;
    yylval=newnode;
    return STAR;
    }
"/" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"DIV\0");
    newnode->judge=0;
    yylval=newnode;
    return DIV;
    }
"&&" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"AND\0");
    newnode->judge=0;
    yylval=newnode;
    return AND;
    }
"||" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"OR\0");
    newnode->judge=0;
    yylval=newnode;
    return OR;
    }
"!" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"NOT\0");
    newnode->judge=0;
    yylval=newnode;
    return NOT;
    }
"(" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"LP\0");
    newnode->judge=0;
    yylval=newnode;
    return LP;
    }
")" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"RP\0");
    newnode->judge=0;
    yylval=newnode;
    return RP;
    }
"[" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"LB\0");
    newnode->judge=0;
    yylval=newnode;
    return LB;
    }
"]" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"RB\0");
    newnode->judge=0;
    yylval=newnode;
    return RB;
    }
"{" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"LC\0");
    newnode->judge=0;
    yylval=newnode;
    return LC;
    }
"}" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"RC\0");
    newnode->judge=0;
    yylval=newnode;
    return RC;
    }
">" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=3;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
"<" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=2;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
">=" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=5;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
"<=" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=4;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
"==" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
"!=" {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=1;
    strcpy(newnode->name,"RELOP\0");
    newnode->judge=0;
    yylval=newnode;
    return RELOP;
    }
{IGNORE} {  }
{CHANGELINE} {yycolumn=1;}
{TYPE} {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"TYPE\0");
    newnode->judge=0;
    strcpy(newnode->char_name,yytext);
    yylval=newnode;
    return TYPE;
    }
{INT} {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"INT\0");
    newnode->judge=0;
    newnode->int_number=atoi(yytext);
    yylval=newnode;
    return INT;
    }
{FLOAT} {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"FLOAT\0");
    newnode->judge=0;
    newnode->float_number=atof(yytext);
    yylval=newnode;
    return FLOAT;
    }
{ID} {
    struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
    newnode->child=NULL;
    newnode->brother=NULL;
    newnode->linenumber=0;
    strcpy(newnode->name,"ID\0");
    newnode->judge=0;
    strcpy(newnode->char_name,yytext);
    yylval=newnode;
    return ID;
    }
. {
    printf("Error type A at Line %d: Mysterious charaters \'%s\'.\n",yylineno,yytext);
    is_error++;
    }
%%

