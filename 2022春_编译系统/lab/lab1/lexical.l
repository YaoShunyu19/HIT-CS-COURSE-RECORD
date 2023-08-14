%{
#include <stdio.h>
#include <stdlib.h>
#include "syntax_tree.h"
#include "syntax.tab.h"
%}
%option yylineno

/*int type*/
INT_OCT [+-]?0[1-7][0-7]*
INT_DEC [+-]?0|[1-9][0-9]*
INT_HEX [+-]?0[xX][a-fA-F0-9]+
INT_BIN [+-]?0[bB][01]+
/*int errors*/
INT_HEX_ERROR [+-]?0[xX][a-fA-F0-9]*[g-zG-Z]+[a-fA-F0-9]*
INT_OCT_ERROR [+-]?0[0-7]*[89]+[0-7]*
INT_BIN_ERROR [+-]?0[bB][01]*[2-9]+[01]*
INT {INT_HEX}|{INT_DEC}|{INT_OCT}|{INT_BIN}|{INT_HEX_ERROR}|{INT_OCT_ERROR}|{INT_BIN_ERROR}
/*float type*/
FLOAT ([+-]?([0-9]*\.?[0-9]+|[0-9]+\.))|([+-]?[0-9]*\.[0-9]*[eE][+-]?[0-9]+)

ID [_a-zA-Z][a-zA-Z_0-9]*
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
/*symbols*/
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
/*key words*/
TYPE "int"|"float"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"

SPACE [ \f\r\t\v]+
COMMENT ("//".*)|("/*"([*]*(([^*/])+([/])*)*)*"*/")
%%
{INT_HEX_ERROR} {printf("Error type A at line %d: Illegal hexadecimal number %s\n",yylineno, yytext);}
{INT_OCT_ERROR} {printf("Error type A at line %d: Illegal octal number %s\n",yylineno, yytext);}
{INT_BIN_ERROR} {printf("Error type A at line %d: Illegal binary number %s\n",yylineno, yytext);}
{INT} {yylval.type_tnode=newST("INT",0,yylineno); return INT;}
{FLOAT} {yylval.type_tnode=newST("FLOAT",0,yylineno); return FLOAT;}
{SEMI} {yylval.type_tnode=newST("SEMI",0,yylineno); return SEMI;}
{COMMA} {yylval.type_tnode=newST("COMMA",0,yylineno); return COMMA;}
{ASSIGNOP} {yylval.type_tnode=newST("ASSIGNOP",0,yylineno); return ASSIGNOP;}
{RELOP} {yylval.type_tnode=newST("RELOP",0,yylineno); return RELOP;}
{PLUS} {yylval.type_tnode=newST("PLUS",0,yylineno); return PLUS;}
{MINUS} {yylval.type_tnode=newST("MINUS",0,yylineno); return MINUS;}
{STAR} {yylval.type_tnode=newST("STAR",0,yylineno); return STAR;}
{DIV} {yylval.type_tnode=newST("DIV",0,yylineno); return DIV;}
{AND} {yylval.type_tnode=newST("AND",0,yylineno); return AND;}
{OR} {yylval.type_tnode=newST("OR",0,yylineno); return OR;}
{DOT} {yylval.type_tnode=newST("DOT",0,yylineno); return DOT;}
{NOT} {yylval.type_tnode=newST("NOT",0,yylineno); return NOT;}
{TYPE} {yylval.type_tnode=newST("TYPE",0,yylineno);return TYPE;}
{LP} {yylval.type_tnode=newST("LP",0,yylineno); return LP;}
{RP} {yylval.type_tnode=newST("RP",0,yylineno); return RP;}
{LB} {yylval.type_tnode=newST("LB",0,yylineno); return LB;}
{RB} {yylval.type_tnode=newST("RB",0,yylineno); return RB;}
{LC} {yylval.type_tnode=newST("LC",0,yylineno); return LC;}
{RC} {yylval.type_tnode=newST("RC",0,yylineno); return RC;}
{STRUCT} {yylval.type_tnode=newST("STRUCT",0,yylineno);return STRUCT;}
{RETURN} {yylval.type_tnode=newST("RETURN",0,yylineno); return RETURN;}
{IF} {yylval.type_tnode=newST("IF",0,yylineno);return IF;}
{ELSE} {yylval.type_tnode=newST("ELSE",0,yylineno); return ELSE;}
{WHILE} {yylval.type_tnode=newST("WHILE",0,yylineno); return WHILE;}
{ID} {yylval.type_tnode=newST("ID",0,yylineno); return ID;}
\n {}
{SPACE} {}
{COMMENT} {}
. {
	is_wrong=1;
	printf("Error type A at line %d: Mystirious charachter %s\n",yylineno,yytext);
}
%%
