#ifndef SEMANTIC_H
#define SEMANTIC_H
#include<stdio.h>
#include<stdlib.h>
#include"node.h"

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
struct Type_
{
    enum {BASIC,ARRAY,STRUCTURE} kind;
    union{
        int basic;//0表示int 1表示float
        struct {Type elem;int size;} array;//元素类型+数组大小
        FieldList structure;//结构体类型
    }u;
};
struct FieldList_
{
    char* name;
    Type type;
    FieldList tail;
};
struct TABLE{
    //0为是，1为否
    //结构体定义时的名字会放入符号表，但它不是一个可以使用的变量
    int is_def_struct;
    FieldList field;
    struct TABLE* next_for_openhash;
    int linenumber;
    int variable;//用于lab3的变量,记录其t后的数据
    int is_from_func;//用于lab3的variable以及address类型的区分
    //0为是，1为否
};
struct FUNCTION{
    char* name;//函数名
    FieldList field;//参数列表
    Type return_type;//返回值
    int declaration;//声明个数
    int definition;//定义个数
    int linenumber;//用于错误打印
    struct FUNCTION* next_for_openhash;
};

unsigned int hash_pjw(char* name);
struct TABLE* find_struct_in_table(char* name);
struct FUNCTION* find_in_function(char* name);
struct FUNCTION* add_to_function(struct FUNCTION* func);
int conflict_between_functions(struct FUNCTION* funca,struct FUNCTION* funcb);
int fieldcmp(FieldList a,FieldList b);
int typecmp(Type a,Type b);
int fieldcmp_forfunc(FieldList a,FieldList b);
//void add_to_table(FieldList fiel,int line,int is_def);
Type find_domain_in_struct(Type type,struct Node* now);
int find_param_in_function(struct FUNCTION* func,struct Node* now);
void add_readwrite_tofunc();
void add_to_table(FieldList fiel,int line,int is_def,int is_from_fun);

void Program(struct Node* now);
void ExtDef(struct Node* now);
Type Specifier(struct Node* now);
Type StructSpecifier(struct Node* now);
//judge=0,从struct语句中来
//judge=1,从变量语句中来，extdeclist\compst
//judge=2,从函数定义语句中来,paramdec
//1&2的区别在于，1需要加入符号表，2不需要加入符号表
FieldList DefList(struct Node* now,int judge);
FieldList Def(struct Node* now,int judge);
FieldList DecList(struct Node* now,Type type,int judge);
FieldList Dec(struct Node* now,Type type,int judge);
//judge=2函数定义，judge=3函数声明
FieldList VarDec(struct Node* now,Type type,int judge);
void ExtDecList(struct Node* now,Type type);
//judge=0，函数声明
//在error判断以及是否加入符号表中会用到
//judge=1,函数定义
void FunDec(struct Node* now,Type type,int judge);
FieldList VarList(struct Node* now,int judge);
FieldList ParamDec(struct Node* now,int judge);
void CompSt(struct Node* now,Type type);
void StmtList(struct Node* now,Type type);
void Stmt(struct Node* now,Type type);
Type Exp(struct Node* now);
FieldList Args(struct Node* now);

#endif