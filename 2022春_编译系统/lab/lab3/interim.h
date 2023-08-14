#ifndef INTERIM_H
#define INTERIM_H
#include<stdio.h>
#include<stdlib.h>
#include"semantic.h"


typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;

struct Operand_{
    enum{FROM_ARG,VARIABLE,TEMP,CONSTANT,ADDRESS,WADDRESS,FUNCTION,LABEL,RELOP}kind;
    int u_int;//t1t2
    char* u_char;
    Type type;//用于结构体和数组变量的Offset查询
};

struct InterCode_{
    enum{ILABEL,IFUNCTION,ASSIGN,
        ADD,SUB,MUL,DIV,
        ADDRASS1,ADDRASS2,ADDRASS3,
        GOTO,IF,RETURN,DEC,ARG,
        CALL,PARAM,READ,WRITE}kind;
    union{
        struct{Operand op;}ulabel;
        struct{Operand op1,op2;}uassign;
        struct{Operand result,op1,op2;}ubinop;
        struct{Operand x,relop,y,z;}uif;
        struct{Operand op;int size;}udec;
    }u;
    InterCode before;
    InterCode next;
};


void translate_print(FILE* f);  //print inter code
void translate_Program(struct Node* now,FILE* F);
void translate_ExtDef(struct Node* now);
void translate_FunDec(struct Node* now);
void translate_CompSt(struct Node* now);
void translate_DefList(struct Node* now);
void translate_StmtList(struct Node* now);
void translate_Def(struct Node* now);
void translate_Stmt(struct Node* now);
void translate_DecList(struct Node* now);
void translate_Exp(struct Node* now,Operand place);
void translate_CompSt(struct Node* now);
void translate_Cond(struct Node* now,Operand lt,Operand lf);
void translate_Dec(struct Node* now);
void translate_VarDec(struct Node* now,Operand place);
void translate_Args(struct Node* now,InterCode here);
void translate_Specifier(struct Node* now);
void translate_StructSpecifier(struct Node* now);



void add_to_intercode(InterCode this);
Operand new_temp();
Operand new_label();

int get_offset(Type return_type,struct Node* after);
int get_size(Type type);

#endif
