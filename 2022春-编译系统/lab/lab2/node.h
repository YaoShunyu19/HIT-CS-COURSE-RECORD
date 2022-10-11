#ifndef NODE_H
#define NODE_H

#include<stdlib.h>
struct Node{
    struct Node* child;//孩子节点
    struct Node* brother;//右侧兄弟节点
    int linenumber;//记录行号，用于打印
    char name[30];//记录该单元规定的名字(token)
    int judge;//judge=0为词法单元，judge=1为语法单元，用于输出判断
    union {
        char char_name[30];
        int int_number;
        float float_number;
    };//记录该单元附加信息int-float-idname,int/float’number
    //语法单元由空串生成的，记录其int_number=0,其余的记为1
};

#endif

