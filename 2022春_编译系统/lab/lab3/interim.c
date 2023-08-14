#include"interim.h"

int lab_num=1;//用于LABEL的命名
int interim_is_error=0;//程序无错=0，有错=1
InterCode in_head=NULL;//记录双向链表头
InterCode in_now=NULL;//current双向链表
extern int temp_num;
//对于符号表中的符号，在加入table时就赋予其数字
int b=0;
int c=0;



void translate_print(FILE* f){
    InterCode temp=in_head;
    int judge_is_null=0;
    int a=0;

    while(temp!=NULL){
        if(temp->kind==ILABEL){
            fprintf(f,"LABEL label");
            fprintf(f,"%d ",temp->u.ulabel.op->u_int);
            fprintf(f,":");
        }
        else if(temp->kind==IFUNCTION){
            fprintf(f,"FUNCTION ");
            fprintf(f,"%s ",temp->u.ulabel.op->u_char);
            fprintf(f,":");
        }
        else if(temp->kind==ASSIGN){
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1==NULL||t2==NULL){
                judge_is_null=1;
            }
            else if(t1->kind==ADDRESS&&t2->kind==ADDRESS){
                fprintf(f,"*t%d",t1->u_int);
                fprintf(f," := ");
                fprintf(f,"*t%d",t2->u_int);
            }
            else if(t2->kind==ADDRESS){
                fprintf(f,"t%d",t1->u_int);
                fprintf(f," := ");
                fprintf(f,"*t%d",t2->u_int);
            }
            else if(t1->kind==ADDRESS){
                fprintf(f,"&t%d",t1->u_int);
                fprintf(f," := ");
                if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
                else fprintf(f,"t%d",t2->u_int);
            }
            else{
                fprintf(f,"t%d",t1->u_int);
                fprintf(f," := ");
                if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
                else fprintf(f,"t%d",t2->u_int);
            }
        }
        else if(temp->kind==ADD){
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," + ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==SUB){
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," - ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==MUL){
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," * ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==DIV){
            Operand t1=temp->u.ubinop.result;
            Operand t2=temp->u.ubinop.op1;
            Operand t3=temp->u.ubinop.op2;
            if(t1==NULL);
            else{
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," / ");
            if(t3->kind==CONSTANT) fprintf(f,"#%d",t3->u_int);
            else fprintf(f,"t%d",t3->u_int);}
        }
        else if(temp->kind==ADDRASS2){
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            fprintf(f,"*t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS3){
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"*t%d",t1->u_int);
            fprintf(f," := ");
            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
        }
        else if(temp->kind==ADDRASS1){
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            fprintf(f,"t%d",t1->u_int);
            fprintf(f," := ");
            fprintf(f,"&t%d",t2->u_int);
        }
        else if(temp->kind==GOTO){
            fprintf(f,"GOTO label");
            fprintf(f,"%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==IF){
            Operand t1=temp->u.uif.x;
            Operand re=temp->u.uif.relop;
            Operand t2=temp->u.uif.y;
            Operand t3=temp->u.uif.z;
            fprintf(f,"IF ");
            if(t1->kind==CONSTANT) fprintf(f,"#%d",t1->u_int);
            else if(t1->kind==ADDRESS) fprintf(f,"*t%d",t1->u_int);
            else fprintf(f,"t%d",t1->u_int);

            if(re->u_int==0) fprintf(f," == ");
            else if(re->u_int==1) fprintf(f," != ");
            else if(re->u_int==2) fprintf(f," < ");
            else if(re->u_int==3) fprintf(f," > ");
            else if(re->u_int==4) fprintf(f," <= ");
            else if(re->u_int==5) fprintf(f," >= ");

            if(t2->kind==CONSTANT) fprintf(f,"#%d",t2->u_int);
            else if(t2->kind==ADDRESS) fprintf(f,"*t%d",t2->u_int);
            else fprintf(f,"t%d",t2->u_int);
            fprintf(f," GOTO label");
            fprintf(f,"%d",t3->u_int);
        }
        else if(temp->kind==RETURN){
            fprintf(f,"RETURN ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==DEC){
            fprintf(f,"DEC ");
            fprintf(f,"t%d ",temp->u.udec.op->u_int);
            fprintf(f,"%d",temp->u.udec.size);
        }
        else if(temp->kind==ARG){
            fprintf(f,"ARG ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==ADDRESS) fprintf(f,"&t%d",temp->u.ulabel.op->u_int);
            else if(temp->u.ulabel.op->kind==WADDRESS) fprintf(f,"*t%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==CALL){
            Operand t1=temp->u.uassign.op1;
            Operand t2=temp->u.uassign.op2;
            if(t1!=NULL) fprintf(f,"t%d := ",t1->u_int);
            else{
                Operand t0=new_temp();
                fprintf(f,"t%d := ",t0->u_int);
            }
            fprintf(f,"CALL ");

            fprintf(f,"%s",t2->u_char);
        }
        else if(temp->kind==PARAM){
            fprintf(f,"PARAM ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==READ){
            fprintf(f,"READ ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        else if(temp->kind==WRITE){
            fprintf(f,"WRITE ");
            if(temp->u.ulabel.op->kind==CONSTANT) fprintf(f,"#%d",temp->u.ulabel.op->u_int);
            else fprintf(f,"t%d",temp->u.ulabel.op->u_int);
        }
        if(judge_is_null==0) fprintf(f,"\n");
        else judge_is_null=0;
        temp=temp->next;
    }

}

void translate_Program(struct Node* now,FILE* F){
    if(interim_is_error==1) return;
    struct Node* extdeflist=now->child;
    while(extdeflist->child!=NULL){
       struct Node* extdef=extdeflist->child;
       translate_ExtDef(extdef);
       extdeflist=extdef->brother;
    }

    if(interim_is_error==0){
        translate_print(F);
    }
}

void translate_ExtDef(struct Node* now){
    /*
    ExtDef → Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    */
    if(interim_is_error==1) return;

    struct Node* after_specifier=now->child->brother;
    if(strcmp(after_specifier->name,"FunDec\0")==0){
        translate_FunDec(after_specifier);
        translate_CompSt(after_specifier->brother);
    }
    else if(strcmp(now->child->name,"Specifier\0")==0){
        translate_Specifier(now->child);
    }
}

void translate_FunDec(struct Node* now){
    //FunDec → ID LP VarList RP| ID LP RP
    if(interim_is_error==1) return;
    //翻译id，生成语句
    Operand func_op=(Operand)malloc(sizeof(struct Operand_));
    func_op->kind=FUNCTION;
    func_op->u_char=now->child->char_name;
    InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));  //FUNCTION
    func_in->kind=IFUNCTION;
    func_in->u.ulabel.op=func_op;
    add_to_intercode(func_in);
    //翻译varlist，参数列表
    if(now->child->brother->brother->brother==NULL) return;
    struct FUNCTION* find_func=find_in_function(now->child->char_name);
    FieldList func_field=find_func->field;
    while(func_field!=NULL){
        //1.每一个参数生成一个对应的operand，根据结构体、非结构体设置kind
        Operand field_op=(Operand)malloc(sizeof(struct Operand_));
        if(func_field->type->kind==STRUCTURE||func_field->type->kind==ARRAY) field_op->kind=ADDRESS;
        else field_op->kind=VARIABLE;
        //记录用户给的名称，以及符号表中赋予的Num
        struct TABLE* find_it=find_struct_in_table(func_field->name);
        field_op->u_int=find_it->variable;
        field_op->u_char=func_field->name;
        //2.每个参数生成一条中间代码
        InterCode field_in=(InterCode)malloc(sizeof(struct InterCode_));  //params
        field_in->kind=PARAM;
        field_in->u.ulabel.op=field_op;
        add_to_intercode(field_in);
        func_field=func_field->tail;
    }
}

void translate_CompSt(struct Node* now){
    //CompSt → LC DefList StmtList RC
    if(interim_is_error==1) return;
    struct Node* deflist=now->child->brother;
    struct Node* stmtlist=deflist->brother;
    translate_DefList(deflist);
    translate_StmtList(stmtlist);
    return;
}

void translate_DefList(struct Node* now){
    //DefList → Def DefList|
    if(interim_is_error==1) return;
    struct Node* newdef=now->child;
    while(newdef!=NULL){
        translate_Def(newdef);
        newdef=newdef->brother->child;
    }
}

void translate_StmtList(struct Node* now){
    //StmtList → Stmt StmtList|
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    while(child!=NULL){
        translate_Stmt(child);
        child=child->brother->child;
    }
}

void translate_Def(struct Node* now){
    //Def → Specifier DecList SEMI
    if(interim_is_error==1) return;
    struct Node* declist=now->child->brother;
    translate_DecList(declist);
}

void translate_Stmt(struct Node* now){
/*   
Stmt → Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt
*/
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    if(strcmp(child->name,"Exp\0")==0){
        //Stmt → Exp SEMI
        translate_Exp(now->child,NULL);
    }
    else if(strcmp(child->name,"CompSt\0")==0){
        //Stmt →CompSt
        translate_CompSt(now->child);
    }
    else if(strcmp(child->name,"RETURN\0")==0){
        //Stmt →RETURN Exp SEMI
        Operand t1=new_temp();
        translate_Exp(now->child->brother,t1);
        Operand t0=NULL;
        if(t1->kind==ADDRESS){
            t0=new_temp();
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADDRASS2;
            code0_in->u.uassign.op1=t0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in);
        }

        InterCode return_in=(InterCode)malloc(sizeof(struct InterCode_));
        return_in->kind=RETURN;
        if(t0==NULL) return_in->u.ulabel.op=t1;
        else return_in->u.ulabel.op=t0;
        add_to_intercode(return_in);
    }
    else if(strcmp(child->name,"IF\0")==0){
        //Stmt →IF LP Exp RP Stmt|IF LP Exp RP Stmt ELSE Stmt
        struct Node* else_node=child->brother->brother->brother->brother->brother;
        if(else_node==NULL){
            //Stmt →IF LP Exp RP Stmt
            Operand l1=new_label();
            Operand l2=new_label();
            translate_Cond(child->brother->brother,l1,l2);
            InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
            l1_in->kind=ILABEL;
            l1_in->u.ulabel.op=l1;
            add_to_intercode(l1_in);
            translate_Stmt(child->brother->brother->brother->brother);
            InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
            l2_in->kind=ILABEL;
            l2_in->u.ulabel.op=l2;
            add_to_intercode(l2_in);     
            return;      
        }
        else{
            Operand l1=new_label();
            Operand l2=new_label();
            Operand l3=new_label();
            translate_Cond(child->brother->brother,l1,l2);
            InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
            l1_in->kind=ILABEL;
            l1_in->u.ulabel.op=l1;
            add_to_intercode(l1_in);
            translate_Stmt(child->brother->brother->brother->brother);
            InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
            l2_in->kind=GOTO;
            l2_in->u.ulabel.op=l3;
            add_to_intercode(l2_in); 
            InterCode l3_in=(InterCode)malloc(sizeof(struct InterCode_));
            l3_in->kind=ILABEL;
            l3_in->u.ulabel.op=l2;
            add_to_intercode(l3_in); 
            translate_Stmt(child->brother->brother->brother->brother->brother->brother);
            InterCode l4_in=(InterCode)malloc(sizeof(struct InterCode_));
            l4_in->kind=ILABEL;
            l4_in->u.ulabel.op=l3;
            add_to_intercode(l4_in); 
            return;
        }
    }
    else if(strcmp(child->name,"WHILE\0")==0){
        //Stmt →WHILE LP Exp RP Stmt
        Operand l1=new_label();
        Operand l2=new_label();
        Operand l3=new_label();
        InterCode l1_in=(InterCode)malloc(sizeof(struct InterCode_));
        l1_in->kind=ILABEL;
        l1_in->u.ulabel.op=l1;
        add_to_intercode(l1_in);
        translate_Cond(child->brother->brother,l2,l3);
        InterCode l3_in=(InterCode)malloc(sizeof(struct InterCode_));
        l3_in->kind=ILABEL;
        l3_in->u.ulabel.op=l2;
        add_to_intercode(l3_in); 
        translate_Stmt(child->brother->brother->brother->brother);
        InterCode l2_in=(InterCode)malloc(sizeof(struct InterCode_));
        l2_in->kind=GOTO;
        l2_in->u.ulabel.op=l1;
        add_to_intercode(l2_in); 
        InterCode l4_in=(InterCode)malloc(sizeof(struct InterCode_));
        l4_in->kind=ILABEL;
        l4_in->u.ulabel.op=l3;
        add_to_intercode(l4_in); 
    }   
}

void translate_DecList(struct Node* now){
//DecList → Dec| Dec COMMA DecList
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    translate_Dec(child);
    struct Node* comma=child->brother;
    if(comma==NULL);
    else translate_DecList(comma->brother);
}

void translate_Exp(struct Node* now,Operand place){
    if(interim_is_error==1) return;
    struct Node* child=now->child;
    struct Node* fir_bro=child->brother;
    if(fir_bro==NULL){
        if(strcmp(child->name,"INT\0")==0){
            //Exp → INT
            int value=child->int_number;
            if(place==NULL) return;
            place->kind=CONSTANT;
            place->u_int=value;
            return;
        }
        else if(strcmp(child->name,"ID\0")==0){
            //Exp → ID
            struct TABLE* find_id=find_struct_in_table(child->char_name);
            char* name=child->char_name;
            if(place==NULL) return;
            if(find_id->is_from_func==0&&find_id->field->type->kind!=BASIC)
                 place->kind=ADDRESS;
            else place->kind=VARIABLE;
            place->u_char=name;
            place->u_int=find_id->variable;
            place->type=find_id->field->type;
            return;
        }
    }
    struct Node* sec_bro=fir_bro->brother;
    if(sec_bro==NULL){
        if(strcmp(child->name,"MINUS\0")==0){
            //Exp → MINUS Exp
            if(place==NULL) return;
            Operand t1=new_temp();
            translate_Exp(child->brother,t1);
            InterCode minus_in=(InterCode)malloc(sizeof(struct InterCode_));
            minus_in->kind=SUB;
            minus_in->u.ubinop.result=place;
            minus_in->u.ubinop.op2=t1;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=0;
            minus_in->u.ubinop.op1=t2;
            if(place->kind==FROM_ARG) place->kind=VARIABLE;
            add_to_intercode(minus_in);    
            return;      
        }
        else if(strcmp(child->name,"NOT\0")==0){
            //Exp →NOT Exp
            Operand l1=new_label();
            Operand l2=new_label();
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ASSIGN;
            code0_in->u.uassign.op1=place;
            Operand t1=(Operand)malloc(sizeof(struct Operand_));
            t1->kind=CONSTANT;
            t1->u_int=0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in); 
            translate_Cond(now,l1,l2);
            InterCode code2_in=(InterCode)malloc(sizeof(struct InterCode_));
            code2_in->kind=ILABEL;
            code2_in->u.ulabel.op=l1;
            add_to_intercode(code2_in);
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=1;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            InterCode label_in=(InterCode)malloc(sizeof(struct InterCode_));
            label_in->kind=ILABEL;
            label_in->u.ulabel.op=l2;
            add_to_intercode(label_in);   
            return; 
        }
    }
    if(strcmp(fir_bro->name,"ASSIGNOP\0")==0){
        //Exp → Exp ASSIGNOP Exp
        if(strcmp(child->child->name,"ID\0")==0){
            Operand t1=new_temp();
            translate_Exp(fir_bro->brother,t1);
            InterCode code21_in=(InterCode)malloc(sizeof(struct InterCode_));
            code21_in->kind=ASSIGN;
            code21_in->u.uassign.op2=t1;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            struct TABLE* find_it=find_struct_in_table(child->child->char_name);
            t2->kind=VARIABLE;
            t2->u_char=child->child->char_name;
            t2->u_int=find_it->variable;
            code21_in->u.uassign.op1=t2;
            add_to_intercode(code21_in);
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            return;
        }
        else{
            Operand t1=new_temp();
            translate_Exp(fir_bro->brother,t1);
            Operand t0=NULL;
            if(t1->kind==ADDRESS){
                t0=new_temp();
                InterCode in_1=(InterCode)malloc(sizeof(struct InterCode_));
                in_1->kind=ADDRASS2;
                in_1->u.uassign.op1=t0;
                in_1->u.uassign.op2=t1;
                add_to_intercode(in_1);
            }
            InterCode code21_in=(InterCode)malloc(sizeof(struct InterCode_));
            code21_in->kind=ADDRASS3;
            if(t0==NULL) code21_in->u.uassign.op2=t1;
            else code21_in->u.uassign.op2=t0;
            Operand t2=new_temp();
            translate_Exp(child,t2);
            code21_in->u.uassign.op1=t2;
            add_to_intercode(code21_in);
            //code22
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            return;
        }
    }
    if(strcmp(fir_bro->name,"AND\0")==0||strcmp(fir_bro->name,"OR\0")==0||strcmp(fir_bro->name,"RELOP\0")==0){
        //Exp → Exp AND\OR\RELOP Exp
            Operand l1=new_label();
            Operand l2=new_label();
            //code0
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ASSIGN;
            code0_in->u.uassign.op1=place;
            Operand t1=(Operand)malloc(sizeof(struct Operand_));
            t1->kind=CONSTANT;
            t1->u_int=0;
            code0_in->u.uassign.op2=t1;
            add_to_intercode(code0_in); 
            //code1
            translate_Cond(now,l1,l2);
            //code2-1
            InterCode code2_in=(InterCode)malloc(sizeof(struct InterCode_));
            code2_in->kind=ILABEL;
            code2_in->u.ulabel.op=l1;
            add_to_intercode(code2_in);
            //code2-2
            InterCode code22_in=(InterCode)malloc(sizeof(struct InterCode_));
            code22_in->kind=ASSIGN;
            code22_in->u.uassign.op1=place;
            Operand t2=(Operand)malloc(sizeof(struct Operand_));
            t2->kind=CONSTANT;
            t2->u_int=1;
            code22_in->u.uassign.op2=t2;
            add_to_intercode(code22_in);
            //LABEL l2
            InterCode label_in=(InterCode)malloc(sizeof(struct InterCode_));
            label_in->kind=ILABEL;
            label_in->u.ulabel.op=l2;
            add_to_intercode(label_in);   
            return;
    }
    if(strcmp(fir_bro->name,"PLUS\0")==0){
        //Exp → Exp PLUS Exp
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=ADD;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"MINUS\0")==0){
        //Exp → Exp MINUS Exp
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=SUB;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"STAR\0")==0){
        //Exp → Exp STAR Exp
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=MUL;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"DIV\0")==0){
        //Exp → Exp DIV Exp
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(fir_bro->brother,t2);
        Operand t3=NULL;
        Operand t4=NULL;
        if(t1->kind==CONSTANT&&t2->kind==CONSTANT) place->kind=VARIABLE;
        if(t1->kind==ADDRESS){
            t3=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t3;
            in1->u.uassign.op2=t1;
            add_to_intercode(in1);
        }
        if(t2->kind==ADDRESS){
            t4=new_temp();
            InterCode in1=(InterCode)malloc(sizeof(struct InterCode_));
            in1->kind=ADDRASS2;
            in1->u.uassign.op1=t4;
            in1->u.uassign.op2=t2;
            add_to_intercode(in1);
        }
        InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
        code0_in->kind=DIV;
        code0_in->u.ubinop.result=place;
        if(t3==NULL) code0_in->u.ubinop.op1=t1;
        else code0_in->u.ubinop.op1=t3;
        if(t4==NULL) code0_in->u.ubinop.op2=t2;
        else code0_in->u.ubinop.op2=t4;
        add_to_intercode(code0_in);
        return; 
    }
    if(strcmp(fir_bro->name,"DOT\0")==0){
        //Exp → Exp DOT ID,结构体
        //1.获取结构体的首地址 t2=t1+#4, t1\v1->address
        Operand t1=new_temp();
        translate_Exp(child,t1);
        if(t1->kind==VARIABLE){
            //0.有address和variable的区别
            Operand t0=new_temp();
            t0->kind=ADDRESS;
            InterCode in=(InterCode)malloc(sizeof(struct InterCode_));
            in->kind=ADDRASS1;
            in->u.uassign.op1=t0;
            in->u.uassign.op2=t1;
            add_to_intercode(in);
            //2.获取当前domain的偏移量
            int offset=get_offset(t1->type,fir_bro->brother);
            if(offset==-1) return;
            Operand t3=(Operand)malloc(sizeof(struct Operand_));
            t3->kind=CONSTANT;
            t3->u_int=offset;
            //3.计算获得现在的地址值，打印中间代码
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            struct TABLE* F=find_struct_in_table(fir_bro->brother->char_name);
            place->type=F->field->type;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t0;
            code0_in->u.ubinop.op2=t3;
            add_to_intercode(code0_in);
        }
        else{
            //2.获取当前domain的偏移量

            int offset=get_offset(t1->type,fir_bro->brother);
            if(offset==-1) return;
            Operand t3=(Operand)malloc(sizeof(struct Operand_));
            t3->kind=CONSTANT;
            t3->u_int=offset;
            //3.计算获得现在的地址值，打印中间代码
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            struct TABLE* F=find_struct_in_table(fir_bro->brother->char_name);
            place->type=F->field->type;

            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t1;
            code0_in->u.ubinop.op2=t3;
            add_to_intercode(code0_in);
        }
        return;
    }
    if(strcmp(fir_bro->name,"LB\0")==0){
        //Exp → Exp LB Exp RB，数组
        //1.获取数组的首地址
        Operand t1=new_temp();
        translate_Exp(child,t1);
        if(t1->kind==VARIABLE){
            //0.有address和variable的区别
            //需要增加一个&d的操作
            Operand t0=new_temp();
            t0->kind=ADDRESS;
            InterCode in=(InterCode)malloc(sizeof(struct InterCode_));
            in->kind=ADDRASS1;
            in->u.uassign.op1=t0;
            in->u.uassign.op2=t1;
            add_to_intercode(in);
            //2.获取数组单个单位的长度offset
            int offset=get_offset(t1->type,NULL);
            if(offset==-1) return;
            //3.生成t2=offset*i
            Operand i=new_temp();
            translate_Exp(fir_bro->brother,i);
            Operand t2=new_temp();
            Operand o_offset=(Operand)malloc(sizeof(struct Operand_));
            o_offset->kind=CONSTANT;
            o_offset->u_int=offset;
            InterCode in2=(InterCode)malloc(sizeof(struct InterCode_));
            in2->kind=MUL;
            in2->u.ubinop.result=t2;
            in2->u.ubinop.op1=i;
            in2->u.ubinop.op2=o_offset;
            add_to_intercode(in2);
            //4.生成中间代码t3=t1+t2
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            place->type=t1->type->u.array.elem;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t0;
            code0_in->u.ubinop.op2=t2;
            add_to_intercode(code0_in);
        }
        else{
            //2.获取数组单个单位的长度offset,生成t2=offset*i
            int offset=get_offset(t1->type,NULL);
            if(offset==-1) return;
            Operand i=new_temp();
            translate_Exp(fir_bro->brother,i);
            Operand t2=new_temp();
            Operand o_offset=(Operand)malloc(sizeof(struct Operand_));
            o_offset->kind=CONSTANT;
            o_offset->u_int=offset;
            InterCode in2=(InterCode)malloc(sizeof(struct InterCode_));
            in2->kind=MUL;
            in2->u.ubinop.result=t2;
            in2->u.ubinop.op1=i;
            in2->u.ubinop.op2=o_offset;
            add_to_intercode(in2);
            //4.生成中间代码t3=t1+t2
            if(place==NULL) return;
            if(place->kind==FROM_ARG) place->kind=WADDRESS;
            else place->kind=ADDRESS;
            place->type=t1->type->u.array.elem;
            InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
            code0_in->kind=ADD;
            code0_in->u.ubinop.result=place;
            code0_in->u.ubinop.op1=t1;
            code0_in->u.ubinop.op2=t2;
            add_to_intercode(code0_in);
        }
        return;
    }
    if(strcmp(fir_bro->name,"Exp\0")==0){
        //Exp → LP Exp RP
        translate_Exp(fir_bro,place);
        return;
    }
    if(strcmp(sec_bro->name,"Args\0")==0){
        //Exp → ID LP Args RP
        //函数调用，有参数
        //0.区分是否为read/write
        char* function_name=child->char_name;
        if(strcmp(function_name,"write")==0){
            Operand t1=new_temp();
            translate_Exp(sec_bro->child,t1);
            //要对地址类的取地址处理
            Operand t0=NULL;
            if(t1->kind==ADDRESS){
                t0=new_temp();
                InterCode code0_in=(InterCode)malloc(sizeof(struct InterCode_));
                code0_in->kind=ADDRASS2;
                code0_in->u.uassign.op1=t0;
                code0_in->u.uassign.op2=t1;
                add_to_intercode(code0_in);
            }
            InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));  //write
            code1_in->kind=WRITE;
            if(t0==NULL) code1_in->u.ulabel.op=t1;
            else code1_in->u.ulabel.op=t0;
            add_to_intercode(code1_in);
            return;
        }
        //1.ARG传递参数，逆序
        translate_Args(sec_bro,NULL);
        //1.2创建函数operand
        Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
        func_ope->kind=FUNCTION;
        func_ope->u_char=function_name;
        //2.place=CALL FUNCTION,保存返回值
        InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
        code1_in->kind=CALL;
        code1_in->u.uassign.op1=place;
        code1_in->u.uassign.op2=func_ope;
        add_to_intercode(code1_in);
        return;
    }
    if(strcmp(sec_bro->name,"RP\0")==0){
        //Exp → ID LP RP
        //没有参数的函数调用
        //0.区分是否为read/write
        char* function_name=child->char_name;
        if(strcmp(function_name,"read")==0){  //read
            InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
            code1_in->kind=READ;
            code1_in->u.ulabel.op=place;
            add_to_intercode(code1_in);
            return;
        }
        //1.2创建函数operand
        Operand func_ope=(Operand)malloc(sizeof(struct Operand_));
        func_ope->kind=FUNCTION;
        func_ope->u_char=function_name;
        //2.place=CALL FUNCTION,保存返回值
        InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
        code1_in->kind=CALL;
        code1_in->u.uassign.op1=place;
        code1_in->u.uassign.op2=func_ope;
        add_to_intercode(code1_in);
        return;
    }
}

void translate_Cond(struct Node* now,Operand lt,Operand lf){
    if(interim_is_error==1) return;
    struct Node* child=now->child;

    if(child->brother!=NULL&&strcmp(child->brother->name,"RELOP\0")==0){
        Operand t1=new_temp();
        Operand t2=new_temp();
        translate_Exp(child,t1);
        translate_Exp(child->brother->brother,t2);
        Operand op=(Operand)malloc(sizeof(struct Operand_));
        op->kind=RELOP;
        op->u_int=child->brother->linenumber;
        InterCode code3=(InterCode)malloc(sizeof(struct InterCode_));
        code3->kind=IF;
        code3->u.uif.x=t1;
        code3->u.uif.relop=op;
        code3->u.uif.y=t2;
        code3->u.uif.z=lt;
        add_to_intercode(code3);
        InterCode code4=(InterCode)malloc(sizeof(struct InterCode_));
        code4->kind=GOTO;
        code4->u.ulabel.op=lf;
        add_to_intercode(code4);
    }
    else if(strcmp(child->name,"NOT\0")==0){
        translate_Cond(now->child->brother,lf,lt);
    }
    else if(child->brother!=NULL&&strcmp(child->brother->name,"AND\0")==0){
        Operand l1=new_label();
        translate_Cond(child,l1,lf);

        InterCode code1=(InterCode)malloc(sizeof(struct InterCode_));
        code1->kind=ILABEL;
        code1->u.ulabel.op=l1;
        add_to_intercode(code1);
        translate_Cond(child->brother->brother,lt,lf);
    }
    else if(child->brother!=NULL&&strcmp(child->brother->name,"OR\0")==0){
        Operand l1=new_label();
        translate_Cond(child,lt,l1);
        InterCode code1=(InterCode)malloc(sizeof(struct InterCode_));
        code1->kind=ILABEL;
        code1->u.ulabel.op=l1;
        add_to_intercode(code1);
        translate_Cond(child->brother->brother,lt,lf);
    }
    else{
        Operand t1=new_temp();
        translate_Exp(now,t1);
        Operand op=(Operand)malloc(sizeof(struct Operand_));
        op->kind=RELOP;
        op->u_int=1;
        Operand t2=(Operand)malloc(sizeof(struct Operand_));
        t2->kind=CONSTANT;
        t2->u_int=0;
        InterCode code2=(InterCode)malloc(sizeof(struct InterCode_));
        code2->kind=IF;
        code2->u.uif.x=t1;
        code2->u.uif.relop=op;
        code2->u.uif.y=t2;
        code2->u.uif.z=lt;
        add_to_intercode(code2);
        InterCode code4=(InterCode)malloc(sizeof(struct InterCode_));
        code4->kind=GOTO;
        code4->u.ulabel.op=lf;
        add_to_intercode(code4);
    }
    return;
}

void translate_Dec(struct Node* now){
    //Dec → VarDec| VarDec ASSIGNOP Exp
    if(interim_is_error==1) return;
    struct Node* vardec=now->child;
    struct Node* fir_bro=vardec->brother;
    if(fir_bro==NULL){
        Operand t1=new_temp();
        translate_VarDec(vardec,t1);
    }
    else{
        Operand rem1=new_temp();
        Operand rem2=new_temp();
        translate_VarDec(vardec,rem1);
        translate_Exp(fir_bro->brother,rem2);
        
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=ASSIGN;
        func_in->u.uassign.op1=rem1;
        func_in->u.uassign.op2=rem2;
        add_to_intercode(func_in);
    }
}

void translate_VarDec(struct Node* now,Operand place){
    //VarDec → ID| VarDec LB INT RB
    //会生成DEC语句
    //只会出现一维数组，所以在else语句中直接处理数组
    if(interim_is_error==1) return;
    if(now->child->brother==NULL){
        //可能是普通变量，可能是structure
        struct TABLE* find_it=find_struct_in_table(now->child->char_name);
        FieldList find_field=find_it->field;
        if(find_field->type->kind==BASIC){
            if(place==NULL) return;
            place->kind=VARIABLE;
            place->u_int=find_it->variable;
            place->u_char=now->child->char_name;
            return;
        }
        else if(find_field->type->kind==STRUCTURE){
            if(place==NULL) return;
            place->kind=VARIABLE;
            place->u_int=find_it->variable;
            place->u_char=now->child->char_name;
            //生成dec语句
            InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
            func_in->kind=DEC;
            func_in->u.udec.op=place;
            func_in->u.udec.size=get_size(find_field->type);
            add_to_intercode(func_in);
            return;
        }
    }
    else{
        //只能是一维数组
        //1.出现多维数组，输出error
        if(strcmp(now->child->child->name,"ID\0")!=0){
            interim_is_error=1;
            printf("Cannot translate: Code contains variables of error array type.\n");
            return;
        }
        if(place==NULL) return;
        place->kind=VARIABLE;
        place->u_char=now->child->char_name;
        struct TABLE* find_it=find_struct_in_table(now->child->child->char_name);
        place->u_int=find_it->variable;
        //生成DEC语句
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=DEC;
        func_in->u.udec.op=place;
        func_in->u.udec.size=get_size(find_it->field->type);
        add_to_intercode(func_in);
        return;
    }
}

void translate_Args(struct Node* now,InterCode here){
//Args → Exp COMMA Args| Exp
    if(interim_is_error==1) return;
    struct Node* exp=now->child;

    Operand t1=new_temp();
    t1->kind=FROM_ARG;
    translate_Exp(exp,t1);

    if(t1->kind==CONSTANT||t1->kind==ADDRESS||t1->kind==WADDRESS);
    else{
        struct TABLE* find_id=find_struct_in_table(t1->u_char);
        if(find_id==NULL) t1->kind=VARIABLE;
        else if(find_id->field->type->kind!=BASIC) t1->kind=ADDRESS;
        else t1->kind=VARIABLE;
    }

    InterCode code1_in=(InterCode)malloc(sizeof(struct InterCode_));
    code1_in->kind=ARG;
    code1_in->u.ulabel.op=t1;

    if(here==NULL) {
        code1_in->next=NULL;
        code1_in->before=NULL;
        }
    else{
        here->next=code1_in;
        code1_in->before=here;
        }
    exp=exp->brother;
    if(exp==NULL);
    else translate_Args(exp->brother,code1_in);
    if(here==NULL){
        InterCode temp=code1_in;
        while(temp->next!=NULL) temp=temp->next;
        while(temp!=NULL){
            InterCode rem=temp;
            temp=temp->before;
            add_to_intercode(rem);}
        }
}

void translate_Specifier(struct Node* now){
    //Specifier → TYPE| StructSpecifier
    if(interim_is_error==1) return;
    if(strcmp(now->child->name,"StructSpecifier")==0){
        translate_StructSpecifier(now->child);
    }
}

void translate_StructSpecifier(struct Node* now){
    //StructSpecifier → STRUCT OptTag LC DefList RC| STRUCT Tag
    if(interim_is_error==1) return;
    struct Node* struct_s=now->child;
    struct Node* fir_bro=struct_s->brother;
    struct Node* sec_bro=fir_bro->brother;
    if(sec_bro==NULL){
        Operand t1=(Operand)malloc(sizeof(struct Operand_));
        struct TABLE* find=find_struct_in_table(fir_bro->child->char_name);
        t1->kind=VARIABLE;
        t1->u_int=find->variable;
        t1->u_char=fir_bro->child->char_name;
        InterCode func_in=(InterCode)malloc(sizeof(struct InterCode_));
        func_in->kind=DEC;
        func_in->u.udec.op=t1;
        func_in->u.udec.size=get_size(find->field->type);
        add_to_intercode(func_in);
        return;
    }
}



void add_to_intercode(InterCode this){
    this->before=NULL;
    this->next=NULL;
    if(in_head==NULL) {
        in_head=this;
        b++;
    }
    if(in_now==NULL) in_now=this;
    else{
        in_now->next=this;
        this->before=in_now;
        in_now=this;
    }
}

Operand new_temp(){
    Operand now=(Operand)malloc(sizeof(struct Operand_));
    now->kind=TEMP;
    now->u_int=temp_num;
    now->u_char="shunyu\0";
    temp_num++;
    return now;
}

Operand new_label(){
    Operand now=(Operand)malloc(sizeof(struct Operand_));
    now->kind=LABEL;
    now->u_int=lab_num;
    lab_num++;
    return now;
}

//计算部分偏移
int get_offset(Type return_type,struct Node* after){
    if(return_type==NULL) return 0;
    if(return_type->kind==BASIC) return 4;
    else if(return_type->kind==STRUCTURE){
        int offset=0;
        FieldList count=return_type->u.structure;
        while(count!=NULL){
            if(strcmp(count->name,after->char_name)==0) break;
            offset=offset+get_size(count->type);
            count=count->tail;
        }
        return offset;
    }
    else if(return_type->kind==ARRAY){
        if(return_type->u.array.elem->kind==ARRAY){
            interim_is_error=1;
            printf("Cannot translate: Code contains variables of error array type.\n");
            return -1;
        }
        int offset=0;
        Type elem=return_type->u.array.elem;
        offset=get_size(elem);
        return offset;
    }

}

//计算全部偏移，即type中的所有内存都要加上
int get_size(Type type){
    if(type==NULL) return 0;
    if(type->kind==BASIC) return 4;
    //只有一维数组
    else if(type->kind==ARRAY){
        int temp=type->u.array.size*get_size(type->u.array.elem);
        return temp;
    }
    else if(type->kind==STRUCTURE){
        int offset=0;
        FieldList count=type->u.structure;
        while(count!=NULL){
            offset=offset+get_size(count->type);
            count=count->tail;
        }
        return offset;
    }
}

