#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <set>

using namespace std;

typedef struct TreeNode
{
    char val;
    struct TreeNode * left;
    struct TreeNode * right;
}TreeNode;

TreeNode * CreateBinaryTree(string input);
void preorder(TreeNode * start);
void inorder(TreeNode * start);
void postorder(TreeNode * start);
void encode(TreeNode * start);
vector<char> SearchandPrint(char c,TreeNode * start);
void decode(TreeNode * start);

int main()
{
    cout<<"请输入二叉树:";
    string input;
    cin>>input;
    TreeNode * start=new TreeNode;
    start=CreateBinaryTree(input);

    cout<<"前序遍历：";
    preorder(start);
    cout<<endl;
    cout<<"中序遍历：";
    inorder(start);
    cout<<endl;
    cout<<"后序遍历：";
    postorder(start);
    cout<<endl;

    int in_number;
    cout<<"请选择（0为明文，1为电码，-1退出）：";
    cin>>in_number;

    switch(in_number)
    {
    case -1:
        exit(0);
        break;
    case 0:
        encode(start);
        break;
    case 1:
        decode(start);
        break;
    default:
        cout<<"wrong!";
        break;
    }
    return 0;
}


TreeNode * CreateBinaryTree(string input)
{
    TreeNode * start=new TreeNode;
    start->left=NULL;
    start->right=NULL;
    start->val=input.at(0);
    stack<TreeNode *> s;
    bool isleft=true;
    s.push(start);
    int sizeofinput=input.size();
    for (int i=1;i<sizeofinput;i++)
    {
        if (input.at(i)!='#')
        {
            TreeNode * node=new TreeNode;
            node->left=NULL;
            node->right=NULL;
            node->val=input.at(i);
            if (isleft==true) s.top()->left=node;
            else if (isleft==false) s.top()->right=node;
            s.push(node);
            isleft=true;
        }
        else if (input.at(i)=='#')
        {
            if (isleft==true) isleft=false;
            else if (isleft==false)
            {
                s.pop();
                while (s.empty()==false && s.top()->right!=NULL) s.pop();
            }
        }
    }
    return start;
}

void preorder(TreeNode * start)
{
    if (start==NULL) return;
    cout<<start->val;
    preorder(start->left);
    preorder(start->right);
}

void inorder(TreeNode * start)
{
    if (start==NULL) return;
    inorder(start->left);
    cout<<start->val;
    inorder(start->right);
}

void postorder(TreeNode * start)
{
    if (start==NULL) return;
    postorder(start->left);
    postorder(start->right);
    cout<<start->val;
}

void encode(TreeNode * start)
{
    cout<<"请输入明文：";
    string pt;
    cin>>pt;
    cout<<"摩斯电码为：";
    for (int i=0;i<(int)pt.size();i++)
    {
        vector<char> morse=SearchandPrint(pt.at(i),start);
        for (int j=0;j<(int)morse.size();j++) cout<<morse.at(j);
        cout<<' ';
    }
}

vector<char> SearchandPrint(char c,TreeNode * start)
{
    vector<char> morse;
    set<char> hasfind;
    stack<TreeNode *> s;
    s.push(start);
    hasfind.insert(start->val);
    while (s.empty()==false)
    {
        while (s.top()->left!=NULL && hasfind.count(s.top()->left->val)==0)
        {
            s.push(s.top()->left);
            hasfind.insert(s.top()->val);
            morse.push_back('.');
            if (s.top()->val==c) return morse;
        }
        if (s.top()->right!=NULL && hasfind.count(s.top()->right->val)==0)
        {
            s.push(s.top()->right);
            hasfind.insert(s.top()->val);
            morse.push_back('-');
            if (s.top()->val==c) return morse;
        }
        else
        {
            s.pop();
            morse.pop_back();
        }
    }
    cout<<"输入有误";
    return morse;
}

void decode(TreeNode * start)
{
    cout<<"请输入电码个数：";
    int n;
    cin>>n;
    cin.ignore();
    cout<<"请输入电码：";
    string cipher;
    getline(cin,cipher);
    TreeNode * p=start;
    cout<<"明文为：";
    for (int i=0;i<(int)cipher.size();i++)
    {
        if (cipher.at(i)==' ')
        {
            cout<<p->val;
            p=start;
        }
        if (cipher.at(i)=='.') p=p->left;
        else if (cipher.at(i)=='-') p=p->right;
        if (p==NULL)
        {
            cout<<"输入有误";
            exit(0);
        }
    }
    cout<<p->val;
}

