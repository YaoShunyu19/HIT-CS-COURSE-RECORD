#include"semantic.h"
#include<string.h>
//均使用指针数组，可以减少内存空间的使用

struct TABLE* table[16384];//储存结构体或变量
struct FUNCTION* function[16384];//储存函数定义或声明
int struct_without_name=0;

void count_table_number(){
    int a=0;
    for(int i=0;i<16384;i++) if(table[i]!=NULL) a++;
}

//查找table表中是否出现name这个名字，出现则返回其TABLE值
struct TABLE* find_struct_in_table(char* name){
    unsigned int val=generate_hash(name);
    if(table[val]==NULL) return NULL;
    else{
        struct TABLE* now=table[val];
        while(now!=NULL){
            if(strcmp(now->field->name,name)==0) return now;
            now=now->next_for_openhash;
        }
    }
    return NULL;
}
//只根据Name进行判断
struct FUNCTION* find_in_function(char* name){
    unsigned int val=generate_hash(name);
    if(function[val]==NULL) return NULL;
    else{
        struct FUNCTION* now=function[val];
        while(now!=NULL){
            if(strcmp(now->name,name)==0) return now;
            now=now->next_for_openhash;
        }
    }
    return NULL;
}

struct FUNCTION* add_to_function(struct FUNCTION* func){
    unsigned int val=generate_hash(func->name);
    if(function[val]==NULL) function[val]=func;
    else{
        struct FUNCTION* rem=function[val];
        while(rem->next_for_openhash!=NULL){
            rem=rem->next_for_openhash;
        }
        rem->next_for_openhash=func;
    }
}
//根据所有参数进行判断，0表示一样，1表示不一样
//只判断参数类型
int conflict_between_functions(struct FUNCTION* funca,struct FUNCTION* funcb){
    if(strcmp(funca->name,funcb->name)!=0) return 1;
    if(fieldcmp(funca->field,funcb->field)!=0) return 1;
    if(typecmp(funca->return_type,funcb->return_type)!=0) return 1;
    return 0;
}
int fieldcmp(FieldList a,FieldList b){
    if(a==NULL&&b==NULL) return 0;
    if(a==NULL||b==NULL) return 1;
    if(strcmp(a->name,b->name)!=0) return 1;
    if(typecmp(a->type,b->type)!=0) return 1;
    if(fieldcmp(a->tail,b->tail)!=0) return 1;
    return 0;
}
int typecmp(Type a,Type b){
    if(a==NULL&&b==NULL) return 0;
    if(a==NULL||b==NULL) return 1;
    if(a->kind==BASIC&&b->kind==BASIC){
        if(a->u.basic==b->u.basic) return 0;
        else return 1;
    }
    else if(a->kind==ARRAY&&b->kind==ARRAY) return typecmp(a->u.array.elem,b->u.array.elem);
    else if(a->kind==STRUCTURE&&b->kind==STRUCTURE) return fieldcmp(a->u.structure,b->u.structure);
    else return 1;
}
int fieldcmp_forfunc(FieldList a,FieldList b){
    if(a==NULL&&b==NULL) return 0;
    if(a==NULL||b==NULL) return 1;
    if(typecmp(a->type,b->type)!=0) return 1;
    if(fieldcmp_forfunc(a->tail,b->tail)!=0) return 1;
    return 0;
}

void add_to_table(FieldList fiel,int line,int is_def){
    int val=generate_hash(fiel->name);
    if(table[val]==NULL){
        table[val]=(struct TABLE*)malloc(sizeof(struct TABLE));
        table[val]->field=fiel;
        table[val]->is_def_struct=is_def;
        table[val]->next_for_openhash=NULL;
        table[val]->linenumber=line;
    }
    else{
        struct TABLE* new_table=(struct TABLE*)malloc(sizeof(struct TABLE));
        new_table->field=fiel;
        new_table->linenumber=line;
        new_table->is_def_struct=is_def;
        new_table->next_for_openhash=NULL;
        struct TABLE* rem=table[val];
        while(rem->next_for_openhash!=NULL){
            rem=rem->next_for_openhash;
        }
        rem->next_for_openhash=new_table;
    }
}

unsigned int generate_hash(char* name){
    unsigned int val=0,i;
    for(;*name;++name){
        val=(val<<2)+*name;
        if(i=val*~0x3fff) val=(val^(i>>12))&0x3fff;
    }
    return val;
}
//有表示找到，NULL表示未找到
Type find_domain_in_struct(Type type,struct Node* now){
    FieldList temp_field=type->u.structure;
    while(temp_field!=NULL){
        if(strcmp(now->char_name,temp_field->name)==0) return temp_field->type;
        temp_field=temp_field->tail;
    }
    return NULL;
}
int find_param_in_function(struct FUNCTION* func,struct Node* now){
    FieldList temp_field=func->field;
    if(now==NULL){
        if(temp_field!=NULL) return 1;
        else return 0;
    }
    else{
        FieldList arg_field=Args(now);
        if(fieldcmp_forfunc(temp_field,arg_field)==0) return 0;
        else return 1;
    }
}

void Program(struct Node* now){
    for(int i=0;i<16384;i++){
        table[i]=NULL;
        function[i]=NULL;
    }
    struct Node* extdeflist=now->child;
    while(extdeflist->child!=NULL){
       struct Node* extdef=extdeflist->child;
       ExtDef(extdef);
       extdeflist=extdef->brother;
    }
}

void ExtDef(struct Node* now){
    /*
    ExtDef → Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    | Specifier FunDec SEMI //add for 2.1
    */
   struct Node* specifier=now->child;
   Type specifier_type=Specifier(specifier);

   struct Node* fir_bro=specifier->brother;
   struct Node* sec_bro=fir_bro->brother;
   if(sec_bro==NULL){
        return;
   }
   else if(strcmp(fir_bro->name,"ExtDecList\0")==0){
       //Specifier ExtDecList SEMI
       //int a;struct complex a;
       //需要把extdeclist与type相关联，放入符号表
       //变量的名字是否被使用
       ExtDecList(fir_bro,specifier_type);
   }
   else if(strcmp(sec_bro->name,"CompSt\0")==0){
       //Specifier FunDec CompSt
       //函数定义，需要把函数相关放入函数表
       FunDec(fir_bro,specifier_type,1);
       //将返回type传递，用于return语句的判断
       CompSt(sec_bro,specifier_type);
   }
   else{
       //Specifier FunDec SEMI
       //函数声明，放入函数表
       FunDec(fir_bro,specifier_type,0);
   }
}

Type Specifier(struct Node* now){
    //Specifier → TYPE| StructSpecifier
    struct Node* child=now->child;
    if(strcmp(child->name,"TYPE\0")==0){
        Type return_type=(Type)malloc(sizeof(struct Type_));
        return_type->kind=BASIC;
        if(strcmp(child->char_name,"int")==0) return_type->u.basic=0;
        else return_type->u.basic=1;
        return return_type;
    }
    else{
        Type return_type=StructSpecifier(child);
        return return_type;
    }
}

Type StructSpecifier(struct Node* now){
    //StructSpecifier → STRUCT OptTag LC DefList RC| STRUCT Tag
   Type return_type=(Type)malloc(sizeof(struct Type_));
   return_type->kind=STRUCTURE;
   struct Node* struct_s=now->child;
   struct Node* fir_bro=struct_s->brother;
   struct Node* sec_bro=fir_bro->brother;
   if(sec_bro==NULL){
       //Tag → ID
       struct TABLE* find_struct=find_struct_in_table(fir_bro->child->char_name);
       if(find_struct==NULL||find_struct->field->type->kind!=STRUCTURE){
           printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",now->linenumber,fir_bro->child->char_name);
           return NULL;
       }
       return_type=find_struct->field->type;
       return return_type;
   }
   else{
       //OptTag → ID|
       FieldList struct_field=(FieldList)malloc(sizeof(struct FieldList_));
       struct Node* opttag=fir_bro;
       struct Node* deflist=opttag->brother->brother;
       //处理opttag
       if(opttag->child==NULL){
           struct_without_name++;
           char name[200]="struct_without_name\0";
           for(int i=0;i<struct_without_name;i++) strcat(name,"a\0");
           struct_field->name=name;
       }
       else{
           //判断是否重复error16
            struct TABLE* find_struct=find_struct_in_table(opttag->child->char_name);
            if(find_struct!=NULL){
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",opttag->linenumber,opttag->child->char_name);
                return NULL;
            }
            struct_field->name=opttag->child->char_name;
       }
       //处理deflist
       return_type->u.structure=DefList(deflist,0);
       //放入符号表中,struct本身放入符号表，且struct的域也要放入符号表
       struct_field->type=return_type;
       struct_field->tail=NULL;
       add_to_table(struct_field,struct_s->linenumber,0);
   }
   return return_type;
}

FieldList DefList(struct Node* now,int judge){
    //DefList → Def DefList|
    struct Node* newdef=now->child;
    FieldList head=NULL;
    FieldList nownode=NULL;
    while(newdef!=NULL){
        FieldList after=Def(newdef,judge);
        if(head==NULL) {
            head=after;
            nownode=after;
        }
        else {
            nownode->tail=after;
            nownode=after;
        }
        if(nownode==NULL) break;
        while(nownode->tail!=NULL) nownode=nownode->tail;
        newdef=newdef->brother->child;
    }
    return head;  
}

FieldList Def(struct Node* now,int judge){
    //Def → Specifier DecList SEMI
    Type specifier_type=Specifier(now->child);
    struct Node* declist=now->child->brother;
    FieldList return_field=NULL;
    return_field=DecList(declist,specifier_type,judge);
    return return_field;
}

FieldList DecList(struct Node* now,Type type,int judge){
    //DecList → Dec| Dec COMMA DecList
    struct Node* dec=now->child;
    struct Node* fir_bro=dec->brother;
    if(fir_bro==NULL){
        FieldList return_field=NULL;
        return_field=Dec(dec,type,judge);
        return return_field;
    }
    else{
        FieldList return_field=NULL;
        return_field=Dec(dec,type,judge);
        if(return_field!=NULL) return_field->tail=DecList(dec->brother->brother,type,judge);
        return return_field; 
    }
}

FieldList Dec(struct Node* now,Type type,int judge){
    //Dec → VarDec| VarDec ASSIGNOP Exp
    struct Node* vardec=now->child;
    struct Node* fir_bro=vardec->brother;
    if(fir_bro==NULL){
        FieldList return_field=NULL;
        return_field=VarDec(vardec,type,judge);
        return return_field;
    }
    else{
        if(judge==0){
            printf("Error type 15 at Line %d: Initialize domain in structure.\n",vardec->linenumber);
            return NULL;
        }
        else{
            Type exp_type=Exp(fir_bro->brother);
            if(typecmp(type,exp_type)==0){ 
                FieldList return_field=NULL;
                return_field=VarDec(vardec,type,judge);
                return return_field;
            }
            else{
                printf("Error type 5 at Line %d: Type mismatched for assignment.\n",now->linenumber);
                return NULL;
            }
        }
    }
}

FieldList VarDec(struct Node* now,Type type,int judge){
    //VarDec → ID| VarDec LB INT RB
    //加入符号表给vardec做,来自函数时不需要加入符号表
    //来自函数定义的符号要加入符号表，来自函数声明的符号不需要加入符号表
    struct Node* child=now->child;
    struct Node* fir_bro=child->brother;
    if(fir_bro==NULL){
        FieldList vardec_field=(FieldList)malloc(sizeof(struct FieldList_));
        vardec_field->name=child->char_name;
        vardec_field->type=type;
        vardec_field->tail=NULL;
        if(judge==3) return vardec_field;
        struct TABLE* find_struct=find_struct_in_table(vardec_field->name);
        if(judge==0&&find_struct!=NULL){
            printf("Error type 15 at Line %d: Redefined field \"%s\".\n",now->linenumber,child->char_name);
            return NULL;
        }
        else if(judge==1&&find_struct!=NULL){
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",now->linenumber,child->char_name);
            return NULL;
        }
        else if(judge==2&&find_struct!=NULL){
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",now->linenumber,child->char_name);
            return NULL;
        }
        add_to_table(vardec_field,child->linenumber,1);
        return vardec_field;
    }
    else{
        struct Node* int_num=fir_bro->brother;
        Type vardec_type=(Type)malloc(sizeof(struct Type_));
        vardec_type->kind=ARRAY;
        vardec_type->u.array.size=int_num->int_number;
        vardec_type->u.array.elem=type;
        FieldList find_upper=VarDec(child,vardec_type,judge);
        return find_upper;
    }
}

void ExtDecList(struct Node* now,Type type){
    //ExtDecList → VarDec| VarDec COMMA ExtDecList
    //extdeclist只有一种可能，直接在这里传1给vardec即可
    //printf("aExtDecList\n");
    struct Node* vardec=now->child;
    struct Node* fir_bro=vardec->brother;
    if(fir_bro==NULL){
        VarDec(vardec,type,1);
    }
    else{
        VarDec(vardec,type,1);
        ExtDecList(fir_bro->brother,type);
    }
}

void FunDec(struct Node* now,Type type,int judge){
    //FunDec → ID LP VarList RP| ID LP RP
    //定义与定义冲突 error4
    //1.新建FUNCTION 2.进行对比 3.error或者加入
    //struct FUNCTION* find_func=find_in_function(now->child->char_name);
    //printf("aFunDec\n");
    //函数声明中的变量不加入变量表
    //只判断类型
    struct FUNCTION* func=(struct FUNCTION*)malloc(sizeof(struct FUNCTION));
    func->name=now->child->char_name;
    func->field=NULL;
    func->return_type=type;
    func->declaration=0;
    func->definition=0;
    func->linenumber=now->linenumber;
    func->next_for_openhash=NULL;
    if(now->child->brother->brother->brother==NULL);
    else{
        func->field=VarList(now->child->brother->brother,judge);
    }
    struct FUNCTION* find_func=find_in_function(func->name);
    if(judge==0){
        if(find_func==NULL){
            func->declaration++;
            add_to_function(func);
        }
        else{
            //声明冲突或多次声明
            int is_conflict=conflict_between_functions(find_func,func);
            if(is_conflict==0) find_func->declaration++;
            else{
                return;
            } 
        }
    }
    else{
        if(find_func==NULL){
            func->definition++;
            add_to_function(func);
        }
        else{
            //定义产生冲突
            int is_conflict=conflict_between_functions(find_func,func);
            if(is_conflict==0){
                //如果找到的find_func与现在的func一致，那么就是重复定义或者第一次定义
                if(func->definition!=0){
                    printf("Error type 4 at Line %d: Redefined function \"%s\".\n",now->linenumber,func->name);
                    return;
                }
                else find_func->definition++;
            }
            else{
                //如果找到的find_func与现在的func不一致，有定义则归结为error4，没有定义再归结为error19
                if(find_func->definition==0){
                    return;}
                else{
                    printf("Error type 4 at Line %d: Redefined function \"%s\".\n",now->linenumber,func->name);
                    return;
                }
            } 
        }
    }
}

FieldList VarList(struct Node* now,int judge){
    //VarList → ParamDec COMMA VarList| ParamDec
    //类似DecList
    struct Node* paramdec=now->child;
    struct Node* fir_bro=paramdec->brother;
    if(fir_bro==NULL){
        FieldList varlist_field=NULL;
        varlist_field=ParamDec(paramdec,judge);
        return varlist_field;
    }
    else{
        FieldList varlist_field=NULL;
        varlist_field=ParamDec(paramdec,judge);
        if(varlist_field!=NULL) varlist_field->tail=VarList(fir_bro->brother,judge);
        return varlist_field; 
    }
}

FieldList ParamDec(struct Node* now,int judge){
    //ParamDec → Specifier VarDec
    Type specifier_type=Specifier(now->child);
    if(judge==0) return VarDec(now->child->brother,specifier_type,3);
    else return VarDec(now->child->brother,specifier_type,2);
}

void CompSt(struct Node* now,Type type){
    //CompSt → LC DefList StmtList RC
    struct Node* deflist=now->child->brother;
    struct Node* stmtlist=deflist->brother;
    DefList(deflist,1);
    StmtList(stmtlist,type);
}

void StmtList(struct Node* now,Type type){
    //StmtList → Stmt StmtList|
    struct Node* child=now->child;
    while(child!=NULL){
        Stmt(child,type);
        child=child->brother->child;
    }
}

void Stmt(struct Node* now,Type type){
    struct Node* child=now->child;
    //printf("aStmt\n");
    if(strcmp(child->name,"Exp\0")==0){
        //Stmt → Exp SEMI
        Exp(now->child);
    }
    else if(strcmp(child->name,"CompSt\0")==0){
        //Stmt →CompSt
        CompSt(now->child,type);
    }
    else if(strcmp(child->name,"RETURN\0")==0){
        //Stmt →RETURN Exp SEMI
        Type exp_type=Exp(child->brother);
        if(typecmp(exp_type,type)==0) return;
        else{
            printf("Error type 8 at Line %d: Type mismatched for return.\n",now->linenumber);
            return;
        }
    }
    else if(strcmp(child->name,"IF\0")==0){
        //Stmt →IF LP Exp RP Stmt|IF LP Exp RP Stmt ELSE Stmt
        struct Node* exp_in_if=child->brother->brother;
        Type exp_type=Exp(exp_in_if);
        if(exp_type==NULL) return;
        if(exp_type->kind==BASIC&&exp_type->u.basic==0);
        else{
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",exp_in_if->linenumber);
            return;
        }
        struct Node* else_node=child->brother->brother->brother->brother->brother;
        if(else_node==NULL){
            Stmt(exp_in_if->brother->brother,type);
        }
        else{
            Stmt(exp_in_if->brother->brother,type);
            Stmt(else_node->brother,type);
        }
    }
    else if(strcmp(child->name,"WHILE\0")==0){
        //Stmt →WHILE LP Exp RP Stmt
        struct Node* exp_in_while=child->brother->brother;
        Type exp_type=Exp(exp_in_while);
        if(exp_type==NULL) return;
        if(exp_type->kind==BASIC&&exp_type->u.basic==0);
        else{
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",exp_in_while->linenumber);
            return;
        }
        Stmt(exp_in_while->brother->brother,type);
    }   
}

Type Exp(struct Node* now){
    struct Node* child=now->child;
    struct Node* fir_bro=child->brother;
    if(fir_bro==NULL){
        if(strcmp(child->name,"INT\0")==0){
            //Exp → INT
            Type return_type=(Type)malloc(sizeof(struct Type_));
            return_type->kind=BASIC;
            return_type->u.basic=0;
            return return_type;
        }
        else if(strcmp(child->name,"FLOAT\0")==0){
            //Exp → FLOAT
            Type return_type=(Type)malloc(sizeof(struct Type_));
            return_type->kind=BASIC;
            return_type->u.basic=1;
            return return_type;
        }
        else if(strcmp(child->name,"ID\0")==0){
            //Exp → ID
            struct TABLE* find_id=find_struct_in_table(child->char_name);
            if(find_id==NULL){
                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",now->linenumber,child->char_name);
                return NULL;
            }
            else{
                if(find_id->is_def_struct==0){
                    printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",now->linenumber,child->char_name);
                    return NULL;
                }
                //如果找到这个id，那就返回这个table的type
                return find_id->field->type;
            }
        }
    }
    struct Node* sec_bro=fir_bro->brother;
    if(sec_bro==NULL){
        if(strcmp(child->name,"MINUS\0")==0){
            //Exp → MINUS Exp
            Type return_type=Exp(fir_bro);
            return return_type;
        }
        else if(strcmp(child->name,"NOT\0")==0){
            //Exp →NOT Exp
            Type return_type=Exp(fir_bro);
            if(return_type==NULL) return NULL;
            if(return_type->kind==BASIC&&return_type->u.basic==0) return return_type;
            else{
                printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
                return NULL;
            }
        }
    }
    if(strcmp(fir_bro->name,"ASSIGNOP\0")==0){
        //Exp → Exp ASSIGNOP Exp
        //对于左值，发现只有vardec与exp有用到assignop，而vardec可以保证为左值
        //child->ID、Exp LB Exp RB以及Exp DOT ID
        //printf("aExp → Exp ASSIGNOP Exp\n");
        Type left_type=Exp(child);
        struct Node* child_child=child->child;
        if(child_child==NULL) return NULL;
        else if(strcmp(child_child->name,"ID\0")==0&&child_child->brother==NULL);
        else if(strcmp(child_child->name,"Exp\0")==0&&strcmp(child_child->brother->name,"LB\0")==0);
        else if(strcmp(child_child->name,"Exp\0")==0&&strcmp(child_child->brother->name,"DOT\0")==0);
        else{
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",now->linenumber);
            return NULL;
        }
        Type right_type=Exp(fir_bro->brother);
        if(typecmp(left_type,right_type)==0) return right_type;
        else {
            if(left_type==NULL||right_type==NULL){
                return NULL;
            }
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"AND\0")==0){
        //Exp → Exp AND Exp
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(left_type==NULL&&right_type==NULL) return NULL;
        if(typecmp(left_type,right_type)==0&&left_type->kind==BASIC&&left_type->u.basic==0) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"OR\0")==0){
        //Exp → Exp OR Exp
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(left_type==NULL&&right_type==NULL) return NULL;
        if(typecmp(left_type,right_type)==0&&left_type->kind==BASIC&&left_type->u.basic==0) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"RELOP\0")==0){
        //Exp → Exp RELOP Exp
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(left_type==NULL||right_type==NULL) return NULL;
        if(typecmp(left_type,right_type)==0) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"PLUS\0")==0){
        //Exp → Exp PLUS Exp
        //算术运算
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(typecmp(left_type,right_type)==0&&left_type!=NULL&&left_type->kind==BASIC) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"MINUS\0")==0){
        //Exp → Exp MINUS Exp
        //算术运算
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(typecmp(left_type,right_type)==0&&left_type!=NULL&&left_type->kind==BASIC) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"STAR\0")==0){
        //Exp → Exp STAR Exp
        //算术运算
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(typecmp(left_type,right_type)==0&&left_type!=NULL&&left_type->kind==BASIC) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"DIV\0")==0){
        //Exp → Exp DIV Exp
        //算术运算
        Type left_type=Exp(child);
        Type right_type=Exp(fir_bro->brother);
        if(typecmp(left_type,right_type)==0&&left_type->kind==BASIC) return right_type;
        else {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",now->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"DOT\0")==0){
        //Exp → Exp DOT ID
        Type return_type=Exp(child);
        if(return_type==NULL) return NULL;
        if(return_type->kind==STRUCTURE);
        else {
            printf("Error type 13 at Line %d: Illegal use of \".\".\n",now->linenumber);
            return NULL;
        }
        Type has_domain=find_domain_in_struct(return_type,fir_bro->brother);
        if(has_domain==NULL){
            printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",now->linenumber,fir_bro->brother->char_name);
            return NULL;
        }
        return has_domain;
    }
    if(strcmp(fir_bro->name,"LB\0")==0){
        //Exp → Exp LB Exp RB
        Type return_type=Exp(child);
        if(return_type==NULL) return NULL;
        if(return_type->kind==ARRAY);
        else {
            printf("Error type 10 at Line %d: \"i\" is not a array.\n",child->linenumber);
            return NULL;
        }
        Type int_type=Exp(child->brother->brother);
        if(int_type==NULL) return NULL;
        if(int_type->kind==BASIC&&int_type->u.basic==0){
            return return_type->u.array.elem;
        }
        else{
            printf("Error type 12 at Line %d: Not integer.\n",child->linenumber);
            return NULL;
        }
    }
    if(strcmp(fir_bro->name,"Exp\0")==0){
        //Exp → LP Exp RP
        Type return_type=Exp(fir_bro);
        return return_type;
    }
    if(strcmp(sec_bro->name,"Args\0")==0){
        //Exp → ID LP Args RP
        struct TABLE* id_table=find_struct_in_table(child->char_name);
        if(id_table!=NULL){
            printf("Error type 11 at Line %d: \"%s\" is not a function.\n",now->linenumber,child->char_name);
            return NULL;
        }
        struct FUNCTION* id_function=find_in_function(child->char_name);
        if(id_function==NULL||id_function->definition==0){
            printf("Error type 2 at Line %d: Undefined function \"%s\".\n",now->linenumber,child->char_name);
            return NULL;
        }
        int judge=find_param_in_function(id_function,sec_bro);
        if(judge==0) return id_function->return_type;
        else {
            printf("Error type 9 at Line %d: Wrong arguments for function \"%s\".\n",now->linenumber,id_function->name);
            return NULL;
        }
    }
    if(strcmp(sec_bro->name,"RP\0")==0){
        //Exp → ID LP RP
        struct TABLE* id_table=find_struct_in_table(child->char_name);
        if(id_table!=NULL){
            printf("Error type 11 at Line %d: \"%s\" is not a function.\n",now->linenumber,child->char_name);
            return NULL;
        }
        struct FUNCTION* id_function=find_in_function(child->char_name);
        if(id_function==NULL||id_function->definition==0){
            printf("Error type 2 at Line %d: Undefined function \"%s\".\n",child->linenumber,child->char_name);
            return NULL;
        }
        int judge=find_param_in_function(id_function,NULL);
        if(judge==0) return id_function->return_type;
        else {
            printf("Error type 9 at Line %d: Wrong arguments for function \"%s\".\n",now->linenumber,id_function->name);
            return NULL;
        }
    }
}

FieldList Args(struct Node* now){
    //Args → Exp COMMA Args| Exp
    struct Node* child=now->child;
    struct Node* fir_bro=child->brother;
    if(fir_bro==NULL){
        FieldList exp_field=(FieldList)malloc(sizeof(struct FieldList_));
        exp_field->type=Exp(child);
        return exp_field;
    }
    else{
        FieldList exp_field=(FieldList)malloc(sizeof(struct FieldList_));
        exp_field->type=Exp(child);
        exp_field->tail=Args(fir_bro->brother);
        return exp_field; 
    }
}
