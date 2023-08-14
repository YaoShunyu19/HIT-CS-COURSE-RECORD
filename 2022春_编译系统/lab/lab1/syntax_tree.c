#include "syntax_tree.h"

int i;

ST newST(char *name, int num, ...)
{
    tnode father = (tnode)malloc(sizeof(struct Node));
    tnode temp = (tnode)malloc(sizeof(struct Node));
    if (!father)
    {
        yyerror("create node error");
        //exit(0);
    }
    father->name = name;

    va_list list;
    va_start(list, num);

    // not terminal symbol
    if (num > 0)
    {
        // first child node
        temp = va_arg(list, tnode);
        setChildTag(temp);
        father->fchild = temp;
        father->line = temp->line;

        if (num >= 2)
        {
            for (i = 0; i < num - 1; i++)
            {
                temp->next = va_arg(list, tnode);
                temp = temp->next;
                // this node is a child
                setChildTag(temp);
            }
        }
    }
    else // terminal symbol or null syntax unit, num is line index
    {
        father->line = va_arg(list, int);
        if ((!strcmp(name, "ID")) || (!strcmp(name, "TYPE")))
        {
            char *str;
            str = (char *)malloc(sizeof(char) * 40);
            strcpy(str, yytext);
            father->id_type = str;
        }
        else if (!strcmp(name, "INT")) father->intval = atoi(yytext);
        else father->fltval = atof(yytext);
    }
    return father;
}

void Preorder(ST st, int level)
{
    if (st != NULL)
    {        
        if (st->line != -1)
        {
            for (i = 0; i < level; i++) printf("  ");
            printf("%s", st->name);
            if ((!strcmp(st->name, "ID")) || (!strcmp(st->name, "TYPE"))) printf(": %s", st->id_type);
            else if (!strcmp(st->name, "INT")) printf(": %d", st->intval);
            else if (!strcmp(st->name, "FLOAT")) printf(": %f", st->fltval);
            else printf("(%d)", st->line);
            printf("\n");
        }
        Preorder(st->fchild, level + 1);
        Preorder(st->next, level);
    }
}

void yyerror(char *msg)
{
    is_wrong = 1;
    fprintf(stderr, "Error type B at Line %d, %s, before %s\n", yylineno, msg, yytext);
}

// set this node is a child
void setChildTag(tnode node)
{
    int i;
    for (i = 0; i < nodeNum; i++) if (nodeList[i] == node) nodeIsChild[i] = 1;
}

int main(int argc, char **argv)
{
    int j;
    if (argc < 2) return 1;
    for (i = 1; i < argc; i++)
    {

        nodeNum = 0;
        memset(nodeList, 0, sizeof(tnode) * 5000);
        memset(nodeIsChild, 0, sizeof(int) * 5000);
        is_wrong = 0;

        FILE *f = fopen(argv[i], "r");
        if (!f)
        {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yyparse();
        fclose(f);

        if (is_wrong)
            continue;
        for (j = 0; j < nodeNum; j++) if (nodeIsChild[j] != 1) Preorder(nodeList[j], 0);
    }
}
