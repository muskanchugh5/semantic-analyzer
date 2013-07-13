#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct prod
{
    char lchild;
    char rchild[10];
    int end;
};

struct node
{
    char c;
    struct node *lchild;
    struct node *rchild;
    struct node *opr;
    int visited;
	int first;
	int inh, syn, val;
};

struct semantic
{
    char c;
    char *action;
    char *end;
};

struct node *mknode(char *, struct prod [], int);

int treeDFS(struct node *, struct semantic []);

void search(struct semantic [], char, struct node *);

int main()
{
    struct prod gram[6] = {'E', "TP", 0, 
                           'P', "+TP", 1,
                           'T', "FQ", 0,
                           'Q', "*FQ", 1,
                           'F', "d", 0,
                           'F', "(E)", 0
			};
    struct semantic rule[9] = {'d', "F.val = id.lexval","",
                               'F', "Q.inh = F.val","",
                               '*', "Q1.inh = Q.inh * F.val","",
                               '+', "P1.inh = P.inh + T.val","",
                               'E', "L.val = E.val","",
                               'T', "P.inh = T.val","",
                               'Q', "Q.syn = Q.inh","T.val = Q.syn",
                               'P', "P.syn = P.inh","E.val = P.syn",
			                   ')', "F.val = E.val", ""
			             };

    struct node *S;
    char expr[10];

    printf("Displaying the grammer of a desktop calculator\n");

    int i;

    for(i = 0; i < 6; i++)
    {
        printf("\n%c -> %s", gram[i].lchild, gram[i].rchild);
        if (gram[i].end)
        {
            printf("|ϵ");
        }
    }
    printf("\nEnter the expression:  ");
    scanf("%s", expr);
    S = mknode(expr, gram, 0);
    treeDFS(S, rule);
    
    printf("\n");
    return 0;
}

struct node* mknode(char *expr, struct prod gram[], int flag)
{
    struct node *t, *p, *f, *q, *id, *e, *temp, *ob, *cb;
    t = p = f = q = id = e = temp = ob = cb = NULL;
    int count = 0;
   
    e = (struct node *)malloc(sizeof(struct node));
    e->c = gram[0].lchild;
    e->first = 0;
    if (flag == 0)
    {
        e->first =1;
        flag = 1;
    }

    e->opr = NULL;
        
    p = (struct node *)malloc(sizeof(struct node));
    p->c = gram[0].rchild[1];
    e->rchild = p;
    p->opr = p->lchild = p->rchild = NULL;
    p->first = 1;
        
    t = (struct node *)malloc(sizeof(struct node));
    t->c = gram[0].rchild[0];
    e->lchild = t;
    t->opr = t->lchild = t->rchild = NULL;
    printf("%c -> %c%c\n", e->c, t->c, p->c);

    int i, j;
    char ch;
    for(i = 0; i < strlen(expr); i++)
    {
        if(isdigit(expr[i]))
        count++;
    }
    i = 0;
    ch = expr[i++];
    if (ch == '(')
    {
        f = (struct node *)malloc(sizeof(struct node));
        t->lchild = f;
        f->c = gram[2].rchild[0];
        f->rchild = f->lchild = NULL;
        f->val = 0;

        q = (struct node *)malloc(sizeof(struct node));
        t->rchild = q;
        q->first = 1;
        q->c = gram[2].rchild[1];
        q->rchild = q->lchild = q->opr = NULL;
        q->val = 0;
        printf("%c -> %c%c\n", t->c, f->c, q->c);

        ob = (struct node *)malloc(sizeof(struct node));
        f->lchild = ob;
        ob->c = gram[5].rchild[0];
        ob->val = 0;
        ob->opr = ob->lchild = ob->rchild = NULL;

        cb = (struct node *)malloc(sizeof(struct node));
        f->rchild = cb;
        cb->c = gram[5].rchild[2];
        cb->val = 0;
        cb->opr = cb->lchild = cb->rchild = NULL;

        printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

        int j = i, k = 0;
        char b_expr[20];
        int ob_count = 0;
        while (expr[j] != ')' || ob_count)
        {
            b_expr[k++] = expr[j++];
            if(b_expr[k-1] == '(')
                ob_count++;
            else if(b_expr[k-1] == ')')
                ob_count--;
            else if(isdigit(b_expr[k-1]))
                count--;
        }
        b_expr[k] = '\0';
        f->opr = mknode(b_expr, gram, flag);
        i = ++j;
        ch = expr[i++];
    }         
    while (count)
    {
        printf("I am stuck here with count = %d and ch = %c\n", count, ch);
        if (isdigit(ch))
        {
            count--;
            f = (struct node *)malloc(sizeof(struct node));
            t->lchild = f;
            f->c = gram[2].rchild[0];
            f->rchild = f->lchild = NULL;
            f->val = 0;

            id = (struct node *)malloc(sizeof(struct node));
            f->opr = id;
            id->opr = id->lchild = id->rchild = NULL;
            id->c = ch;
            id->val = ch - '0';

            q = (struct node *)malloc(sizeof(struct node));
            t->rchild = q;
            q->first = 1;
            q->c = gram[2].rchild[1];
            q->rchild = q->lchild = q->opr = NULL;
            q->val = 0;
            printf("%c -> %c%c\n", t->c, f->c, q->c);
            printf("%c -> %c\n", f->c, id->c);

            ch = expr[i++];
        }
        else if (ch == '+' || ch == '-')
        {
            t = (struct node *)malloc(sizeof(struct node));
            p->lchild = t;
            t->c = gram[1].rchild[1];
            t->rchild = t->lchild = t->opr = NULL;
            t->val = 0;

            id = (struct node *)malloc(sizeof(struct node));
            p->opr = id;
            id->opr = id->lchild = id->rchild = NULL;
            id->c = ch;

            temp = p;
            p = (struct node *)malloc(sizeof(struct node));
            temp->rchild = p;
            p->first = 0;
            p->c = gram[1].rchild[2];
            p->rchild = p->lchild = p->opr = NULL;
            p->val = 0;

            if (q->rchild == NULL && q->lchild == NULL && q->opr == NULL)
                  printf("%c -> ϵ\n", q->c);


            printf("%c -> %c%c%c\n", temp->c, id->c, t->c, p->c);

            ch = expr[i++];
            if (isdigit(ch))
            {
                count--;
                f = (struct node *)malloc(sizeof(struct node));
                t->lchild = f;
                f->c = gram[2].rchild[0];
                f->rchild = f->lchild = NULL;
                f->val = 0;

                id = (struct node *)malloc(sizeof(struct node));
                f->opr = id;
                id->opr = id->lchild = id->rchild = NULL;
                id->c = ch;
                id->val = ch - '0';

                q = (struct node *)malloc(sizeof(struct node));
                t->rchild = q;
                q->first = 1;
                q->c = gram[2].rchild[1];
                q->rchild = q->lchild = q->opr = NULL;
                q->val = 0;
                printf("%c -> %c%c\n", t->c, f->c, q->c);
                printf("%c -> %c\n", f->c, id->c);

                ch = expr[i++];
            }
            else if (ch == '(')
            {
                f = (struct node *)malloc(sizeof(struct node));
                t->lchild = f;
                f->c = gram[2].rchild[0];
                f->rchild = f->lchild = NULL;
                f->val = 0;

                q = (struct node *)malloc(sizeof(struct node));
                t->rchild = q;
                q->first = 1;
                q->c = gram[2].rchild[1];
                q->rchild = q->lchild = q->opr = NULL;
                q->val = 0;
                printf("%c -> %c%c\n", t->c, f->c, q->c);

                ob = (struct node *)malloc(sizeof(struct node));
                f->lchild = ob;
                ob->c = gram[5].rchild[0];
                ob->val = 0;
                ob->opr = ob->lchild = ob->rchild = NULL;

                cb = (struct node *)malloc(sizeof(struct node));
                f->rchild = cb;
                cb->c = gram[5].rchild[2];
                cb->val = 0;
                cb->opr = cb->lchild = cb->rchild = NULL;

                printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                int j = i, k = 0;
                char b_expr[20];
                int ob_count = 0;
                while (expr[j] != ')' || ob_count)
                {
                    b_expr[k++] = expr[j++];
                    if(b_expr[k-1] == '(')
                        ob_count++;
                    else if(b_expr[k-1] == ')')
                        ob_count--;
                    else if(isdigit(b_expr[k-1]))
                        count--;
                }
                b_expr[k] = '\0';
                f->opr = mknode(b_expr, gram, flag);
            }
         }
         else if (ch == '*' || ch == '/')
         {
             f = (struct node *)malloc(sizeof(struct node));
             q->lchild = f;
             f->c = gram[3].rchild[1];
             f->rchild = f->lchild = f->opr = NULL;
             f->val = 0;

             id = (struct node *)malloc(sizeof(struct node));
             q->opr = id;
             id->c = ch;
             id->val = 0;
             id->rchild = id->lchild = id->opr = NULL;

             temp = q;
             q = (struct node *)malloc(sizeof(struct node));
             q->first = 0;
             temp->rchild = q;
             q->c = gram[3].rchild[2];
             q->rchild = q->lchild = q->opr = NULL;
             q->val = 0;

             printf("%c -> %c%c%c\n", temp->c, id->c, f->c, q->c);

             ch = expr[i++];
             if (isdigit(ch))
             {
                 id = (struct node *)malloc(sizeof(struct node));
                 f->opr = id;
                 id->c = ch;
                 id->val = ch - '0';
                 id->opr = id->lchild = id->rchild = NULL;
                 count--;
                 printf("%c -> %c\n", f->c, id->c);
             }
             else if (ch == '(')
             {
                 ob = (struct node *)malloc(sizeof(struct node));
                 f->lchild = ob;
                 ob->c = gram[5].rchild[0];
                 ob->val = 0;
                 ob->opr = ob->lchild = ob->rchild = NULL;

                 cb = (struct node *)malloc(sizeof(struct node));
                 f->rchild = cb;
                 cb->c = gram[5].rchild[2];
                 cb->val = 0;
                 cb->opr = cb->lchild = cb->rchild = NULL;

                 printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                 int j = i, k = 0, ob_count = 0;
                 char b_expr[20];
                 while (expr[j] != ')' || ob_count)
                 {
                     b_expr[k++] = expr[j++];
                     if(b_expr[k-1] == '(')
                         ob_count++;
                     else if(b_expr[k-1] == ')')
                         ob_count--;
                     else if (isdigit(b_expr[k-1]))
                         count--;
                 }
                 b_expr[k] = '\0';
                 f->opr = mknode(b_expr, gram, flag);
            }
         }
     }
     printf("%c -> ϵ\n", q->c);
     printf("%c -> ϵ\n", p->c);

     return e;
}
int treeDFS(struct node *root, struct semantic rule[])
{
    char *str;
    root->visited = 1;

    if (root->lchild)
    {
        if(root->lchild->visited == 0)
        {
            treeDFS(root->lchild, rule);
        }
    }

    if (root->opr)
    {
        if(root->opr->visited == 0)
        {
            treeDFS(root->opr, rule);
        }
    }
    if (root->rchild)
    {
        if(root->rchild->visited == 0)
        {
            treeDFS(root->rchild, rule);
        }
    }
    printf("[%c]\n", root->c);
    search(rule, root->c, root);
}

void search(struct semantic rule[], char a, struct node *root)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (isdigit(a))
            a = 'd';
        if (rule[i].c == 'E' && !root->first)
            break;
        if (rule[i].c == a)
        {
            printf("%s\n", rule[i].action);
            if (root->first == 1)
                printf("%s\n", rule[i].end);
            printf("\n");
            return;
        }
        printf("No action\n");
    }
}
