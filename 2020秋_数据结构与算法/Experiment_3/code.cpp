#include <iostream>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <set>

using namespace std;

typedef struct Vertex
{
    int num;
    bool viewed;
    struct Edge * AdjacentEdge;//此顶点所邻接的边
}Vertex;

typedef struct Edge
{
    int head;
    int tail;
    int weight;
    struct Edge * Attack_Edge;//此边所依附的边
}Edge;

void init_Graph();//初始化图
void prompt();//提示
void InsertEdge(int A,int B,int lengthAB);//插入边
void DeleteEdge(int A,int B);
void Addvertex();//1:添加顶点
void Deletevertex();//2:删除顶点
void Findvertex();//3:查找顶点
void Addedge();//4:添加边
void Deleteedge();//5:删除边
void DFS(int startdfs);//6:深度优先搜索
void BFS(int startbfs);//7:广度优先搜索
void Dijkstra(int start);//8:计算给定顶点的到其他顶点的最短路径
void Floyd();//9:计算图中任意两顶点之间的最短路径
void Checkgraph();//10:查看当前图

int numofvertex;
int numofedge;
vector<Vertex *> Graph;
int main()
{
    init_Graph();
    int choice;
    do
    {
        cout<<endl;
        prompt();
        cout<<"Input your choice:";
        cin>>choice;
        switch (choice)
        {
        case 1:
            Addvertex();
            break;
        case 2:
            Deletevertex();
            break;
        case 3:
            Findvertex();
            break;
        case 4:
            Addedge();
            break;
        case 5:
            Deleteedge();
            break;
        case 6:
            {
                Vertex * v;
                int flag=-1;
                for (int i=0;i<numofvertex;i++)
                {
                    v=Graph[i];
                    v->viewed=false;
                }
                cout<<"Input the start vertex:";
                int startdfs;
                cin>>startdfs;
                for (int i=0;i<numofvertex;i++) if (Graph[i]->num==startdfs) flag=i;
                if (flag==-1)
                {
                    cout<<"Not exist!"<<endl;
                    continue;
                }
                else DFS(startdfs);
                for (int i=0;i<numofvertex;i++)
                {
                    if (Graph[i]->num==startdfs) continue;
                    else if (Graph[i]->viewed==false) DFS(Graph[i]->num);
                }
                cout<<endl;
                break;
            }
        case 7:
            {
                Vertex * v;
                int flag=-1;
                for (int i=0;i<numofvertex;i++)
                {
                    v=Graph[i];
                    v->viewed=false;
                }
                cout<<"Input the start vertex:";
                int startbfs;
                cin>>startbfs;
                for (int i=0;i<numofvertex;i++) if (Graph[i]->num==startbfs) flag=i;
                if (flag==-1)
                {
                    cout<<"Not exist!"<<endl;
                    continue;
                }
                else BFS(startbfs);
                for (int i=0;i<numofvertex;i++)
                {
                    if (Graph[i]->num==startbfs) continue;
                    else if (Graph[i]->viewed==false) BFS(Graph[i]->num);
                }
                cout<<endl;
                break;
            }
        case 8:
            int start;
            cout<<"Input the start vertex:";
            cin>>start;
            Dijkstra(start);
            break;
        case 9:
            Floyd();
            break;
        case 10:
            Checkgraph();
            break;
        case 0:
            break;
        default:
            cout<<"wrong!"<<endl;
            continue;
        }

    }while (choice!=0);
    return 0;
}

void init_Graph()//初始化图
{
    cout<<"Input the number of vertexes and edges:"<<endl;
    cin>>numofvertex>>numofedge;
    for (int i=0;i<numofvertex;i++)
    {
        Vertex * v=new Vertex;
        v->num=i;
        v->AdjacentEdge=NULL;
        Graph.push_back(v);
    }

    int edge[numofedge][3];
    cout<<"Input the information of edges:"<<endl;
    for (int i=0;i<numofedge;i++)
    {
        cin.clear();
        scanf("%d %d %d",&edge[i][0],&edge[i][1],&edge[i][2]);
        if (edge[i][0]>=numofvertex || edge[i][1]>=numofvertex)
        {
            cout<<"wrong!";
            exit(0);
        }
        else
        {
            Edge * e=new Edge;
            e->head=edge[i][0];
            e->tail=edge[i][1];
            e->weight=edge[i][2];
            e->Attack_Edge=NULL;
            InsertEdge(edge[i][0],edge[i][1],edge[i][2]);//无向图邻接表插入两次
            InsertEdge(edge[i][1],edge[i][0],edge[i][2]);
        }
    }
}

void prompt()//提示
{
    cout<<"1:Add vertex;"<<endl;
    cout<<"2:Delete vertex;"<<endl;
    cout<<"3:Find vertex;"<<endl;
    cout<<"4:Add edge;"<<endl;
    cout<<"5:Delete edge;"<<endl;
    cout<<"6:Given a vertex, DFS;"<<endl;
    cout<<"7:Given a vertex, BFS;"<<endl;
    cout<<"8:Given a vertex, calculate the shortest path to other vertexes;"<<endl;
    cout<<"9:Calculate the shortest path between any two vertexes;"<<endl;
    cout<<"10:Check the current graph;"<<endl;
    cout<<"0:Exit."<<endl;
}

void InsertEdge(int A,int B,int lengthAB)//插入边AB
{
    Edge * e=new Edge;
    e->head=A;
    e->tail=B;
    e->weight=lengthAB;
    e->Attack_Edge=NULL;
    if (Graph[A]->AdjacentEdge==NULL) Graph[A]->AdjacentEdge=e;
    else
    {
        e->Attack_Edge=Graph[A]->AdjacentEdge;
        Graph[A]->AdjacentEdge=e;
    }
    return ;
}

void DeleteEdge(int A,int B)//删除边AB
{
    int a;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==A) a=i;
    Edge * e=Graph[a]->AdjacentEdge;
    if (e->tail==B) Graph[a]->AdjacentEdge=e->Attack_Edge;
    else
    {
        while (e->Attack_Edge->tail!=B) e=e->Attack_Edge;
        e->Attack_Edge=e->Attack_Edge->Attack_Edge;
    }
    return ;
}

void Addvertex()//添加顶点
{
    cout<<"Input the number of the vertex:";
    int n;
    cin>>n;
    Vertex * v=new Vertex;
    v->AdjacentEdge=NULL;
    v->num=n;
    numofvertex++;
    Graph.push_back(v);
    cout<<"Added successfully."<<endl;
    return ;
}

void Deletevertex()//删除顶点
{
    cout<<"Input the number of the vertex:";
    int n;
    cin>>n;
    int flag=-1;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==n) flag=i;
    if (flag==-1)
    {
        cout<<"Not exist!"<<endl;
        return ;
    }
    else
    {
        for (int i=0;i<numofvertex;i++)
        {
            Edge * e=Graph[i]->AdjacentEdge;
            while (e!=NULL)
            {
                if (e->head==n || e->tail==n) DeleteEdge(e->head,e->tail);
                e=e->Attack_Edge;
            }
        }
        if (flag==numofvertex-1) Graph.pop_back();
        else
        {
            for (int i=flag+1;i<numofvertex;i++)
            {
                Graph[i-1]=Graph[i];
            }
            Graph.pop_back();
        }
        numofvertex--;
    }
    cout<<"Delete successfully."<<endl;
    return;
}

void Findvertex()//查找顶点
{
    cout<<"Input the number of the vertex:";
    int n;
    cin>>n;
    int flag=-1;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==n) flag=i;
    if (flag==-1)
    {
        cout<<"Not exist!"<<endl;
        return ;
    }
    else
    {
        cout<<"Find it!"<<endl;
        Edge * e=Graph[flag]->AdjacentEdge;
        while (e!=NULL)
        {
            cout<<e->tail<<' ';
            e=e->Attack_Edge;
        }
    }
    cout<<endl;
    return ;
}

void Addedge()//添加边
{
    cout<<"Input the head,tail and weight of the edge:"<<endl;
    int A,B,weight;
    cin>>A>>B>>weight;
    bool eA=false;
    bool eB=false;
    for (int i=0;i<numofvertex;i++)
    {
        if (Graph[i]->num==A) eA=true;
        if (Graph[i]->num==B) eB=true;
    }
    if (eA==false || eB==false)
    {
        cout<<"Not exist!"<<endl;
        return ;
    }
    Edge * e1=new Edge;
    e1->head=A;
    e1->tail=B;
    e1->weight=weight;
    Edge * e2=new Edge;
    e2->head=B;
    e2->tail=A;
    e2->weight=weight;

    Vertex * v;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==A) v=Graph[i];
    e1->Attack_Edge=v->AdjacentEdge;
    v->AdjacentEdge=e1;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==B) v=Graph[i];
    e2->Attack_Edge=v->AdjacentEdge;
    v->AdjacentEdge=e2;
    cout<<"Add successfully."<<endl;
    return ;
}

void Deleteedge()//删除边
{
    cout<<"Input the head and tail of the edge:"<<endl;
    int A,B;
    cin>>A>>B;
    bool eA=false;
    bool eB=false;
    for (int i=0;i<numofvertex;i++)
    {
        if (Graph[i]->num==A) eA=true;
        if (Graph[i]->num==B) eB=true;
    }
    if (eA==false || eB==false)
    {
        cout<<"Not exist!"<<endl;
        return ;
    }
    Edge * e=Graph[A]->AdjacentEdge;
    bool existofedge=false;
    while (e!=NULL)
    {
        if (e->tail==B) existofedge=true;
        e=e->Attack_Edge;
    }
    if (existofedge==false)
    {
        cout<<"Not exist!"<<endl;
        return ;
    }
    DeleteEdge(A,B);
    DeleteEdge(B,A);
    cout<<"Delete successfully."<<endl;
    return ;
}

void DFS(int startdfs)
{
    Vertex * v;
    Vertex * u;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==startdfs) v=Graph[i];
    if (v->viewed==false)
    {
        cout<<v->num<<' ';
        v->viewed=true;
        if (v->AdjacentEdge!=NULL)
        {
            for (int i=0;i<numofvertex;i++) if (Graph[i]->num==v->AdjacentEdge->tail) u=Graph[i];
            if (u->viewed==false) DFS(u->num);
        }
    }
    return ;
}

void BFS(int startbfs)
{
    queue<Vertex *> q;
    Vertex * v;
    Edge * e;
    for (int i=0;i<numofvertex;i++) if (Graph[i]->num==startbfs) v=Graph[i];
    q.push(v);
    v->viewed=true;
    while (q.empty()==false)
    {
        v=q.front();
        cout<<v->num<<' ';
        q.pop();
        e=v->AdjacentEdge;
        while (e!=NULL)
        {
            for (int i=0;i<numofvertex;i++) if (Graph[i]->num==e->tail) v=Graph[i];
            if (v->viewed==false)
            {
                q.push(v);
                v->viewed=true;
            }
            e=e->Attack_Edge;
        }
    }
}

void Dijkstra(int start)
{
    set<int> s;//按编号存，最短路径已经确定的顶点集合
    s.insert(start);
    vector<int> d;//按序号存，从源点到顶点集合中其余各顶点的最短路径长度
    int flag;
    for (int i=0;i<numofvertex;i++)//首先将数组全部初始化为无穷
    {
        if (Graph[i]->num==start)
        {
            d.push_back(60001);
            flag=i;
        }
        else d.push_back(60000);
    }
    Edge * e=Graph[flag]->AdjacentEdge;
    while (e!=NULL)
    {
        for (int i=0;i<numofvertex;i++)
        {
            if (Graph[i]->num==e->tail) d[i]=e->weight;//修改正确的权值
        }
        e=e->Attack_Edge;
    }

    for (int i=0;i<numofvertex-1;i++)
    {
        int minium=0;
        for (int j=0;j<numofvertex;j++)//从最短路径未确定的顶点集合中查找使得到源点距离最小的顶点
        {
            if (s.count(Graph[j]->num)!=0) continue;
            if (d[j]<d[minium]) minium=j;
        }
        s.insert(Graph[minium]->num);
        for (int j=0;j<numofvertex;j++)//遍历未确定最短路径的顶点集合
        {
            if (s.count(Graph[j]->num)!=0) continue;
            int cwj=60000;
            if (Graph[minium]->num==Graph[j]->num) cwj=0;
            Edge * e=Graph[minium]->AdjacentEdge;
            while (e!=NULL)
            {
                if (e->tail==Graph[j]->num) cwj=e->weight;
                e=e->Attack_Edge;
            }
            d[j]=(d[j]<(d[minium]+cwj))?d[j]:(d[minium]+cwj);//更新源点到其余各顶点最短路径长度
        }
    }
    for (int i=0;i<numofvertex;i++)
    {
        if (Graph[i]->num==start) continue;
        cout<<start<<"->"<<Graph[i]->num<<':'<<d[i]<<endl;
    }
}

void Floyd()
{
    int A[numofvertex][numofvertex];
    for (int i=0;i<numofvertex;i++)//构造邻接矩阵
    {
        for (int j=0;j<numofvertex;j++)
        {
            if (i==j) A[i][j]=0;
            else
            {
                A[i][j]=60000;
                Edge * e=Graph[i]->AdjacentEdge;
                while (e!=NULL)
                {
                    if (e->tail==Graph[j]->num) A[i][j]=e->weight;
                    e=e->Attack_Edge;
                }
            }
        }
    }
    for (int k=0;k<numofvertex;k++)
    {
        for (int i=0;i<numofvertex;i++)
        {
            for (int j=0;j<numofvertex;j++)
            {
                if (A[i][k]+A[k][j]<A[i][j]) A[i][j]=A[i][k]+A[k][j];
            }
        }
    }
    for (int i=0;i<numofvertex;i++)
    {
        for (int j=0;j<i;j++)
            cout<<Graph[j]->num<<"->"<<Graph[i]->num<<':'<<A[j][i]<<' ';
        cout<<endl;
    }
}

void Checkgraph()//输出邻接表
{
    Vertex * v;
    Edge * e;
    for (int i=0;i<numofvertex;i++)
    {
        v=Graph[i];
        cout<<"Vertex"<<v->num<<":  ";
        e=v->AdjacentEdge;
        while (e!=NULL)
        {
            cout<<e->head<<','<<e->tail<<':'<<e->weight<<' ';
            e=e->Attack_Edge;
        }
        cout<<endl;
    }
    return ;
}
