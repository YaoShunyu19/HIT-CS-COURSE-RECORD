#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <string>

using namespace std;

typedef struct node
{
    int val;
    struct node * pre;
    struct node * next;
}node;

void headinsert(node * head,int in);//双向链表头部插入
void tailinsert(node * head,int in);//双向链表尾部插入
void reverseprintnode(node * head);//双向链表逆序输出
int listsize(node * head);//双向链表节点数目
int head1head2(node * head_1,node * head_2);//比较两个数的大小
node * resultofadd(node * head_1,node * head_2);//返回加法结果的头指针
node * resultofminus(node * head_1,node * head_2);//返回减法结果的头指针
node * resultofmultiply(node * head_1,node * head_2);//返回乘法结果的头指针
void myfree(node * head);//释放链表空间

int main()
{
    node * head_1=new node;
    node * head_2=new node;
    head_1->pre=NULL;
    head_1->next=NULL;
    head_2->pre=NULL;
    head_2->next=NULL;

    string input;
    cout<<"input the operation expression:";
    cin>>input;//输入字符串

    char op=NULL,op1=NULL,op2=NULL;
    bool occur=false;//记录是否出现运算符
    for (int i=0;i<(int)input.size();i++)//对输入进行遍历
    {
        if (i==0 && input.at(i)=='+') continue;
        if (i==0 && input.at(i)=='-')
        {
            op='-';
            i++;
        }
        if (input.at(i)=='+' || input.at(i)=='-' || input.at(i)=='*')//若出现运算符，记录
        {
            occur=true;
            op1=input.at(i);
            i++;
        }
        if (input.at(i)=='+' || input.at(i)=='-' || input.at(i)=='*')//若出现两个运算符，再次记录
        {
            op2=input.at(i);
            i++;
        }
        if (occur==false) headinsert(head_1,int(input.at(i)-'0'));//出现运算符之前，数字插入到第一个链表中
        else headinsert(head_2,int(input.at(i)-'0'));//出现运算符之后，数字插入到第二个链表中
    }

    bool np=true;//记录结果的正负
    node * re=new node;
    re->pre=NULL;
    re->next=NULL;
    switch(op1)//判断运算符，执行不同函数
    {
    case '+':
        {
            if (op2=='-')
            {
                if (op==NULL)
                {
                    int compare=head1head2(head_1,head_2);//执行减法函数
                    if (compare==0) np=false;//输入的第一个数较小，结果为负
                    re=resultofminus(head_1,head_2);
                }
                else
                {
                    np=false;
                    re=resultofadd(head_1,head_2);
                }
            }
            else
            {
                if (op=='-')
                {
                    int compare=head1head2(head_1,head_2);//执行减法函数
                    if (compare==1) np=false;//输入的第一个数较大，结果为负
                    re=resultofminus(head_1,head_2);
                }
                else
                {
                    re=resultofadd(head_1,head_2);
                }
            }
            break;
        }
    case '-':
        {
            if (op2=='-')
            {
                if (op==NULL) re=resultofadd(head_1,head_2);
                else
                {
                    int compare=head1head2(head_1,head_2);
                    if (compare==1) np=false;//输入的第一个数较大，结果为负
                    re=resultofminus(head_1,head_2);
                }
            }
            else
            {
                if (op=='-')
                {
                    np=false;
                    re=resultofadd(head_1,head_2);
                }
                else
                {
                    int compare=head1head2(head_1,head_2);
                    if (compare==0) np=false;//输入的第一个数较小，结果为负
                    re=resultofminus(head_1,head_2);
                }
            }
            break;
        }
    case '*':
        {
            if (op==NULL && op2=='-' || op=='-' && op2!='-') np=false;
            if (op2=='-' &&
                (head_1->next->val==0 && head_1->next->next==NULL
                 || head_2->next->val==0 && head_2->next->next==NULL)) np=true;
            re=resultofmultiply(head_1,head_2);
            break;
        }
    default:
        {
            cout<<"wrong!";//条件均不满足，输入有误
            exit(0);
        }
    }
    cout<<"result=";
    if (np==false) cout<<'-';//若结果为负，先输出负号
    reverseprintnode(re);//逆序输出结果
    myfree(head_1);//释放空间
    myfree(head_2);
    myfree(re);
}

void headinsert(node * head,int in)
{
    if (head->next==NULL)//若链表只有头节点
    {
        node * n=new node;
        head->next=n;
        n->pre=head;
        n->next=NULL;
        n->val=in;
    }
    else
    {
        node * n=new node;
        n->next=head->next;
        n->pre=head;
        head->next->pre=n;
        head->next=n;
        n->val=in;
    }
}

void tailinsert(node * head,int in)
{
    node * n=new node;
    node * p=new node;
    p=head;
    while (p->next!=NULL) p=p->next;//遍历到链表尾部
    p->next=n;
    n->pre=p;
    n->next=NULL;
    n->val=in;
}

void reverseprintnode(node * head)
{
    node * p=head;
    while (p->next!=NULL) p=p->next;//遍历到链表尾部，从后向前输出
    while (p!=head)
    {
        cout<<p->val;
        p=p->pre;
    }
    cout<<endl;
}

int listsize(node * head)
{
    int ls=0;
    node * p=head->next;
    while (p!=NULL)//遍历链表节点，返回链表长度
    {
        p=p->next;
        ls++;
    }
    return ls;
}

int head1head2(node * head_1,node * head_2)
{
    if (listsize(head_1)>listsize(head_2)) return 1;//长度不同
    else if (listsize(head_1)<listsize(head_2)) return 0;
    else//长度相同
    {
        node * p1=head_1->next;
        node * p2=head_2->next;
        while (p1->next!=NULL) p1=p1->next;//指向链表尾部
        while (p2->next!=NULL) p2=p2->next;
        while (p1->val==p2->val && p1->pre!=head_1)//从后向前遍历，出现不同则退出循环
        {
            p1=p1->pre;
            p2=p2->pre;
        }
        if (p1->val>p2->val) return 1;
        if (p1->val<p2->val) return 0;
        if (p1->val==p2->val) return 2;
    }
}

node * resultofadd(node * head_1,node * head_2)//返回head_1与head_2之和的头节点
{
    node * re=new node;
    re->next=NULL;
    re->pre=NULL;
    node * p1=head_1->next;
    node * p2=head_2->next;
    int value,carry=0,p1val,p2val;
    for (;p1!=NULL || p2!=NULL;)//有一个指针不空时继续相加
    {
        if (p1==NULL) p1val=0;//指针为空则值为0
        else p1val=p1->val;
        if (p2==NULL) p2val=0;
        else p2val=p2->val;
        value=(p1val+p2val+carry)%10;
        carry=(p1val+p2val+carry)/10;
        tailinsert(re,value);
        if (p1==NULL || p1->next==NULL) p1=NULL;
        else p1=p1->next;
        if (p2==NULL || p2->next==NULL) p2=NULL;
        else p2=p2->next;
        if (p1==NULL && p2==NULL && carry!=0) tailinsert(re,carry);//最后一位的进位，直接插入carry进位变量
    }
    return re;
}

node * resultofminus(node * head_1,node * head_2)//返回head_1与head_2之差的头节点
{
    node * re=new node;
    re->next=NULL;
    re->pre=NULL;

    if (head1head2(head_1,head_2)==2)//两数相等
    {
        tailinsert(re,0);
    }
    else if (head1head2(head_1,head_2)==1)//大数减小数，直接相减
    {
        node * p1=head_1->next;
        node * p2=head_2->next;
        int value,carry=0,p1val,p2val;
        for (;p1!=NULL || p2!=NULL;)
        {
            if (p1==NULL) p1val=0;
            else p1val=p1->val;
            if (p2==NULL) p2val=0;
            else p2val=p2->val;
            if (p1val-carry>=p2val)
            {
                value=p1val-p2val-carry;//不借位，carry=0
                carry=0;
            }
            else//借位，carry=1
            {
                value=10+p1val-p2val-carry;
                carry=1;
            }
            tailinsert(re,value);
            if (p1==NULL || p1->next==NULL) p1=NULL;
            else p1=p1->next;
            if (p2==NULL || p2->next==NULL) p2=NULL;
            else p2=p2->next;
        }
        node * p=re;
        while (p->next!=NULL) p=p->next;
        if (p->val==0)//将链表结尾值为0的节点释放，保证输出的第一位不为0
        {
            node * ppre=p->pre;
            ppre->next=NULL;
            free(p);
        }
    }
    else//小数减大数，计算绝对值
    {
        node * p1=head_2->next;
        node * p2=head_1->next;
        int value,carry=0,p1val,p2val;
        for (;p1!=NULL || p2!=NULL;)
        {
            if (p1==NULL) p1val=0;
            else p1val=p1->val;
            if (p2==NULL) p2val=0;
            else p2val=p2->val;
            if (p1val-carry>=p2val)
            {
                value=p1val-p2val-carry;
                carry=0;
            }
            else
            {
                value=10+p1val-p2val-carry;
                carry=1;
            }
            tailinsert(re,value);
            if (p1==NULL || p1->next==NULL) p1=NULL;
            else p1=p1->next;
            if (p2==NULL || p2->next==NULL) p2=NULL;
            else p2=p2->next;
        }
        node * p=re;
        while (p->next!=NULL) p=p->next;
        if (p->val==0)
        {
            node * ppre=p->pre;
            ppre->next=NULL;
            free(p);
        }
    }
    return re;
}

node * resultofmultiply(node * head_1,node * head_2)//返回head_1与head_2之积的头节点
{
    if (head_1->next->val==0 && head_1->next->next==NULL
        || head_2->next->val==0 && head_2->next->next==NULL)
    {
        node * re=new node;
        node * p=new node;
        p->val=0;
        p->val=NULL;
        re->next=p;
        re->pre=NULL;
        p->pre=re;
        return re;
    }
    node * p1=head_1->next;
    node * p2=head_2->next;
    if (head1head2(head_1,head_2)==0)//若head_1<head_2，p1指向head_2，p2指向head_1
    {
        p1=head_2->next;
        p2=head_1->next;
    }
    int value,carry=0,p1val,p2val;

    node * re=new node;
    re->next=NULL;
    re->pre=NULL;
    node * r =new node;

    for (int i=0;p2!=NULL;p2=p2->next,i++)//第一重循环，遍历较小数的每一位
    {
        node * n=new node;
        n->next=NULL;
        n->pre=NULL;
        carry=0;
        p1=head_1->next;
        if (head1head2(head_1,head_2)==0) p1=head_2->next;
        for (;p1!=NULL;p1=p1->next)//较小数的某一位和较大数的每一位做积
        {
            p1val=p1->val;
            p2val=p2->val;
            value=p1val*p2val+carry;
            carry=value/10;
            value%=10;
            tailinsert(n,value);
        }
        if (carry!=0)//若进位不等于0，将carry的每一位数字插入链表中
        {
            int c=carry%10;
            while (carry>=10)
            {
                c=carry%10;
                tailinsert(n,c);
                carry/=10;
            }
            tailinsert(n,carry);
        }
        int j=i;
        while (j>0)
        {
            headinsert(n,0);
            j--;
        }
        r=n;
        re=resultofadd(r,re);//第一重循环每执行一轮，将结果与上一轮的结果求和得到本轮的结果
        r=re;
    }
    return re;
}

void myfree(node * head)
{
	node * p=head->next;
	while (head->next!=NULL)//遍历节点，释放空间
	{
		free(head);
		head=p;
		p=head->next;
	}
	free(head);
}

