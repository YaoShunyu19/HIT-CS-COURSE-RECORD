%{
#include<unistd.h>
#include<stdio.h>   
#include "syntax_tree.h"
%}

%union{
  tnode type_tnode;
  double d;
}

%token <type_tnode> INT FLOAT
%token <type_tnode> TYPE STRUCT RETURN IF ELSE WHILE ID COMMENT SPACE SEMI COMMA ASSIGNOP PLUS
%token <type_tnode> MINUS STAR DIV AND OR DOT NOT LP RP LB RB LC RC AERROR RELOP EOL
%type  <type_tnode> Program ExtDefList ExtDef ExtDecList Specifire StructSpecifire 
%type  <type_tnode> OptTag Tag VarDec FunDec VarList ParamDec Compst StmtList Stmt DefList Def DecList Dec Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT 
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE 
%nonassoc ELSE

%%
/*High-level Definitions*/
Program:ExtDefList {$$=newST("Program",1,$1);nodeList[nodeNum]=$$;nodeNum++;};
ExtDefList:ExtDef ExtDefList {$$=newST("ExtDefList",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	| {$$=newST("ExtDefList",0,-1);nodeList[nodeNum]=$$;nodeNum++;}
	;
ExtDef:Specifire ExtDecList SEMI {$$=newST("ExtDef",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}    
	|Specifire SEMI {$$=newST("ExtDef",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	|Specifire FunDec Compst {$$=newST("ExtDef",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
ExtDecList:VarDec {$$=newST("ExtDecList",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|VarDec COMMA ExtDecList {$$=newST("ExtDecList",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
/*Specifire*/
Specifire:TYPE {$$=newST("Specifire",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|StructSpecifire {$$=newST("Specifire",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	;
StructSpecifire:STRUCT OptTag LC DefList RC {$$=newST("StructSpecifire",5,$1,$2,$3,$4,$5);nodeList[nodeNum]=$$;nodeNum++;}
	|STRUCT Tag {$$=newST("StructSpecifire",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	;
OptTag:ID {$$=newST("OptTag",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|{$$=newST("OptTag",0,-1);nodeList[nodeNum]=$$;nodeNum++;}
	;
Tag:ID {$$=newST("Tag",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	;
/*Declarators*/
VarDec:ID {$$=newST("VarDec",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|VarDec LB INT RB {$$=newST("VarDec",4,$1,$2,$3,$4);nodeList[nodeNum]=$$;nodeNum++;}
	|error "\n" {yyerrok;yyclearin;}
	;
FunDec:ID LP VarList RP {$$=newST("FunDec",4,$1,$2,$3,$4);nodeList[nodeNum]=$$;nodeNum++;}
	|ID LP RP {$$=newST("FunDec",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
VarList:ParamDec COMMA VarList {$$=newST("VarList",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	|ParamDec {$$=newST("VarList",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	;
ParamDec:Specifire VarDec {$$=newST("ParamDec",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	;

/*Statement*/
Compst:LC DefList StmtList RC {$$=newST("Compst",4,$1,$2,$3,$4);nodeList[nodeNum]=$$;nodeNum++;}
	;
StmtList:Stmt StmtList{$$=newST("StmtList",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	| {$$=newST("StmtList",0,-1);nodeList[nodeNum]=$$;nodeNum++;}
	;
Stmt:Exp SEMI {$$=newST("Stmt",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	|Compst {$$=newST("Stmt",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|RETURN Exp SEMI {$$=newST("Stmt",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	|IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=newST("Stmt",				5,$1,$2,$3,$4,$5);nodeList[nodeNum]=$$;nodeNum++;}
	|IF LP Exp RP Stmt ELSE Stmt {$$=newST("Stmt",7,$1,$2,$3,$4,$5,$6,$7);nodeList[nodeNum]=$$;nodeNum++;}
	|WHILE LP Exp RP Stmt {$$=newST("Stmt",5,$1,$2,$3,$4,$5);nodeList[nodeNum]=$$;nodeNum++;}
	|error SEMI {yyerrok;yyclearin;}
	;
/*Local Definitions*/
DefList:Def DefList{$$=newST("DefList",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
	| {$$=newST("DefList",0,-1);nodeList[nodeNum]=$$;nodeNum++;}
	;
Def:Specifire DecList SEMI {$$=newST("Def",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
DecList:Dec {$$=newST("DecList",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|Dec COMMA DecList {$$=newST("DecList",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
Dec:VarDec {$$=newST("Dec",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
	|VarDec ASSIGNOP Exp {$$=newST("Dec",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
	;
/*Expressions*/
Exp:Exp ASSIGNOP Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp AND Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp OR Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp RELOP Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp PLUS Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp MINUS Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp STAR Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp DIV Exp{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |LP Exp RP{$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |MINUS Exp {$$=newST("Exp",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
        |NOT Exp {$$=newST("Exp",2,$1,$2);nodeList[nodeNum]=$$;nodeNum++;}
        |ID LP Args RP {$$=newST("Exp",4,$1,$2,$3,$4);nodeList[nodeNum]=$$;nodeNum++;}
        |ID LP RP {$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp LB Exp RB {$$=newST("Exp",4,$1,$2,$3,$4);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp DOT ID {$$=newST("Exp",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |ID {$$=newST("Exp",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
        |INT {$$=newST("Exp",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
        |FLOAT{$$=newST("Exp",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
        ;
Args:Exp COMMA Args {$$=newST("Args",3,$1,$2,$3);nodeList[nodeNum]=$$;nodeNum++;}
        |Exp {$$=newST("Args",1,$1);nodeList[nodeNum]=$$;nodeNum++;}
        ;
%%
