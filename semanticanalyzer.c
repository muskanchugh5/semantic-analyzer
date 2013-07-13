#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct prod
{
    char left;
    char right[10];
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
void delete(struct node **);
void grammar(struct prod []);

int main()
{
    struct prod gram[6] = {'E', "TP", 0, 
                           'P', "+TP|-TP", 1,
                           'T', "FQ", 0,
                           'Q', "*FQ|/FQ", 1,
                           'F', "d", 0,
                           'F', "(E)", 0
			};
    struct semantic rule[11] = {'d', "F.val = id.lexval","",
                               'F', "Q.inh = F.val","",
                               '*', "Q1.inh = Q.inh * F.val","",
                               '/', "Q1.inh = Q.inh / F.val","",
							   '+', "P1.inh = P.inh + T.val","",
                               '-', "P1.inh = P.inh - T.val","",
							   'T', "P.inh = T.val","",
                               'Q', "Q.syn = Q.inh","T.val = Q.syn",
                               'P', "P.syn = P.inh","E.val = P.syn",
							   'E', "L.val = E.val","",
			  			       ')', "F.val = E.val", ""
			  				  };

    struct node *S = NULL;
    char expr[10] = "";
    int ch, choice = 0;

    do {

    printf("\n\nEnter your choice:\n1. Display the grammar\n2. Enter an expression\n3. Display the parse tree\n4. Display the semantic rule\n5. Delete the Parse Tree\n6. Help\n7. About the developers\n8. Quit\n\nChoice: ");
    scanf("%d", &ch);

    switch (ch)
    {
        case 1: grammar(gram);
                break;

        case 2: printf("\nEnter the expression: ");
                scanf("%s", expr);
                printf("\n\nFollow this sequence to generate the parse tree...\n");
                if (S)
                    delete(&S);
                S = mknode(expr, gram, 0);
                break;

        case 3: if (S)
                {
                    delete(&S);
                    S = mknode(expr, gram, 0);
                }
                else
                    printf("No parse tree exist\n");

                break;

        case 4: if (S)
                {
                     printf("\n\nDisplaying Semantic Rule for each node\n");
                     treeDFS(S, rule);
                }
                else
                    printf("Semantic rule needs a parse tree\n");
                break;

        case 5: if(S)
                {
                    printf("Deleting the parse tree..\n");
                    delete(&S);
                }
                else
                    printf("Parse tree does not exist\n");
                break;

        case 6: printf("The user should enter a mathematical expression using the operators *, /, +, -, ( and ). The program will generate a parse tree. The user has to follow step-by-step with a pen and paper to see the parse tree. After the parse tree is generated, the program finds the semantic rules that will be applied to each node in the parse tree by using DFS traversal.\n");
                break;

        case 7: printf("Developed by:\n1) Sujaya A. Maiya - 1PI10IS106\n2) Prasoon Telang = 1PI10IS113\n3) Vivek Agarwal - 1PI10IS120\nVth-B\nSemantic Analyzer of a Compiler\n");
                break;

        case 8: printf("Are you sure you want to QUIT? [1/0]\n");
                scanf("%d", &choice);
       }
    }while(choice != 1);
    printf("PROGRAM TERMINATED\n");
    return 0;
}

void grammar(struct prod gram[])
{
    printf("Displaying the grammar\n");
    int i;
    for(i = 0; i < 6; i++)
    {
        printf("\n%c -> %s", gram[i].left, gram[i].right);
        if (gram[i].end)
        {
            printf("|ϵ");
        }
    }
    printf("\n");
}
struct node* mknode(char *expr, struct prod gram[], int flag)
{
    struct node *t, *p, *f, *q, *id, *e, *temp, *ob, *cb;
    t = p = f = q = id = e = temp = ob = cb = NULL;
    int count = 0;
    int dflag = 0;
    int oflag = 0;
   
    e = (struct node *)malloc(sizeof(struct node));
    e->c = gram[0].left;
    e->first = 0;
    if (flag == 0)
    {
        e->first =1;
        flag = 1;
    }

    e->opr = e->rchild = e->lchild = NULL;
        
    p = (struct node *)malloc(sizeof(struct node));
    p->c = gram[0].right[1];
    e->rchild = p;
    p->opr = p->lchild = p->rchild = NULL;
    p->first = 1;
        
    t = (struct node *)malloc(sizeof(struct node));
    t->c = gram[0].right[0];
    e->lchild = t;
    t->opr = t->lchild = t->rchild = NULL;
    printf("%c -> %c%c\n", e->c, t->c, p->c);

    int i, j;
    char ch;
    for(i = 0; i < strlen(expr); i++)
    {
        if(isdigit(expr[i]) || isalpha(expr[i]))
        count++;
    }
    i = 0;
    ch = expr[i++];
    if (ch == '(')
    {
        f = (struct node *)malloc(sizeof(struct node));
        t->lchild = f;
        f->c = gram[2].right[0];
        f->rchild = f->lchild = f->opr = NULL;
        f->val = 0;

        q = (struct node *)malloc(sizeof(struct node));
        t->rchild = q;
        q->first = 1;
        q->c = gram[2].right[1];
        q->rchild = q->lchild = q->opr = NULL;
        q->val = 0;
        printf("%c -> %c%c\n", t->c, f->c, q->c);

        ob = (struct node *)malloc(sizeof(struct node));
        f->lchild = ob;
        ob->c = gram[5].right[0];
        ob->val = 0;
        ob->opr = ob->lchild = ob->rchild = NULL;

        cb = (struct node *)malloc(sizeof(struct node));
        f->rchild = cb;
        cb->c = gram[5].right[2];
        cb->val = 0;
        cb->opr = cb->lchild = cb->rchild = NULL;

        printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

        int j = i, k = 0;
        char b_expr[20];
        int ob_count = 0;
        int prev_count = count;
        while (expr[j] != ')' || ob_count)
        {
            b_expr[k++] = expr[j++];
            if(b_expr[k-1] == '(')
                ob_count++;
            else if(b_expr[k-1] == ')')
                ob_count--;
            else if(isdigit(b_expr[k-1]) || isalpha(b_expr[k-1]))
                count--;
        }
        if (ob_count || prev_count == count)
        {
            printf("Invalid ( or content inside () missing\nTry Again\n");
            return NULL;
        }
        b_expr[k] = '\0';
        f->opr = mknode(b_expr, gram, flag);
        i = ++j;
        ch = expr[i++];
    }         
    while (count)
    {
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && !isdigit(ch) && !isalpha(ch))
        {
            printf("Invalid Character\nTry Again\n");
            return NULL;
        }
        if (isdigit(ch) || isalpha(ch))
        {
            if (!dflag)
            {
                dflag = 1;
                oflag = 0;
            }
            else if (dflag)
            {
                printf("Use of multiple digit\nTry Again\n");
                return NULL;
            }
            count--;
            f = (struct node *)malloc(sizeof(struct node));
            t->lchild = f;
            f->c = gram[2].right[0];
            f->rchild = f->opr = f->lchild = NULL;
            f->val = 0;

            id = (struct node *)malloc(sizeof(struct node));
            f->opr = id;
            id->opr = id->lchild = id->rchild = NULL;
            id->c = ch;
            id->val = ch - '0';

            q = (struct node *)malloc(sizeof(struct node));
            t->rchild = q;
            q->first = 1;
            q->c = gram[2].right[1];
            q->rchild = q->lchild = q->opr = NULL;
            q->val = 0;
            printf("%c -> %c%c\n", t->c, f->c, q->c);
            printf("%c -> %c\n", f->c, id->c);

            ch = expr[i++];
        }
        else if (ch == '+' || ch == '-')
        {
            if (!oflag)
            {
                dflag = 0;
                oflag = 1;
            }
            else if (oflag)
            {
                printf("Invalid use of +/-\nTry Again\n");
                return NULL;
            }

            t = (struct node *)malloc(sizeof(struct node));
            p->lchild = t;
            t->c = gram[1].right[1];
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
            p->c = gram[1].right[2];
            p->rchild = p->lchild = p->opr = NULL;
            p->val = 0;

            if (q->rchild == NULL && q->lchild == NULL && q->opr == NULL)
                  printf("%c -> ϵ\n", q->c);


            printf("%c -> %c%c%c\n", temp->c, id->c, t->c, p->c);

            ch = expr[i++];
            if (isdigit(ch) || isalpha(ch))
            {
                if (!dflag)
                {
                    dflag = 1;
                    oflag = 0;
                }
                else if (dflag)
                {
                    printf("Use of multiple digit\nTry Again\n");
                    return NULL;
                }
                count--;
                f = (struct node *)malloc(sizeof(struct node));
                t->lchild = f;
                f->c = gram[2].right[0];
                f->rchild = f->opr = f->lchild = NULL;
                f->val = 0;

                id = (struct node *)malloc(sizeof(struct node));
                f->opr = id;
                id->opr = id->lchild = id->rchild = NULL;
                id->c = ch;
                id->val = ch - '0';

                q = (struct node *)malloc(sizeof(struct node));
                t->rchild = q;
                q->first = 1;
                q->c = gram[2].right[1];
                q->rchild = q->lchild = q->opr = NULL;
                q->val = 0;
                printf("%c -> %c%c\n", t->c, f->c, q->c);
                printf("%c -> %c\n", f->c, id->c);

                ch = expr[i++];
            }
            else if (ch == ')')
            {
                printf("Illegal use of ) after + or -\nTry again\n");
                return NULL;
            }
            else if (ch == '(')
            {
                f = (struct node *)malloc(sizeof(struct node));
                t->lchild = f;
                f->c = gram[2].right[0];
                f->rchild = f->lchild = f->opr = NULL;
                f->val = 0;

                q = (struct node *)malloc(sizeof(struct node));
                t->rchild = q;
                q->first = 1;
                q->c = gram[2].right[1];
                q->rchild = q->lchild = q->opr = NULL;
                q->val = 0;
                printf("%c -> %c%c\n", t->c, f->c, q->c);

                ob = (struct node *)malloc(sizeof(struct node));
                f->lchild = ob;
                ob->c = gram[5].right[0];
                ob->val = 0;
                ob->opr = ob->lchild = ob->rchild = NULL;

                cb = (struct node *)malloc(sizeof(struct node));
                f->rchild = cb;
                cb->c = gram[5].right[2];
                cb->val = 0;
                cb->opr = cb->lchild = cb->rchild = NULL;

                printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                int j = i, k = 0;
                char b_expr[20];
                int ob_count = 0;
                int prev_count = count;
                while (expr[j] != ')' || ob_count)
                {
                    b_expr[k++] = expr[j++];
                    if(b_expr[k-1] == '(')
                        ob_count++;
                    else if(b_expr[k-1] == ')')
                        ob_count--;
                    else if(isdigit(b_expr[k-1]) || isalpha(b_expr[k-1]))
                        count--;
                }
                if (ob_count || prev_count == count)
                {
                     printf("Invalid ) or content inside () missing\nTry Again\n");
                     return NULL;
                }
                b_expr[k] = '\0';
                f->opr = mknode(b_expr, gram, flag);
            }
         }
         else if (ch == '*' || ch == '/')
         {
             if (!oflag)
             {
                 dflag = 0;
                 oflag = 1;
             }
             else if (oflag)
             {
                 printf("Invalid */-\nTry Again\n");
                 return NULL;
             }
             f = (struct node *)malloc(sizeof(struct node));
             q->lchild = f;
             f->c = gram[3].right[1];
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
             q->c = gram[3].right[2];
             q->rchild = q->lchild = q->opr = NULL;
             q->val = 0;

             printf("%c -> %c%c%c\n", temp->c, id->c, f->c, q->c);

             ch = expr[i++];
             if (isdigit(ch) || isalpha(ch))
             {
                 if (!dflag)
                 {
                     dflag = 1;
                     oflag = 0;
                 }
                 else if (dflag)
                 {
                    printf("Use of multiple digit\nTry Again\n");
                    return NULL;
                 }
                 id = (struct node *)malloc(sizeof(struct node));
                 f->opr = id;
                 id->c = ch;
                 id->val = ch - '0';
                 id->opr = id->lchild = id->rchild = NULL;
                 count--;
                 printf("%c -> %c\n", f->c, id->c);
                 ch = expr[i++];
             }
             else if (ch == ')')
             {
                 printf("Illegal use of ) after + or -\nTry again\n");
                 return NULL;
             }
             else if (ch == '(')
             {
                 ob = (struct node *)malloc(sizeof(struct node));
                 f->lchild = ob;
                 ob->c = gram[5].right[0];
                 ob->val = 0;
                 ob->opr = ob->lchild = ob->rchild = NULL;

                 cb = (struct node *)malloc(sizeof(struct node));
                 f->rchild = cb;
                 cb->c = gram[5].right[2];
                 cb->val = 0;
                 cb->opr = cb->lchild = cb->rchild = NULL;

                 printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                 int j = i, k = 0, ob_count = 0;
                 char b_expr[20];
                 int prev_count = count;
                 while (expr[j] != ')' || ob_count)
                 {
                     b_expr[k++] = expr[j++];
                     if(b_expr[k-1] == '(')
                         ob_count++;
                     else if(b_expr[k-1] == ')')
                         ob_count--;
                     else if(isdigit(b_expr[k-1]) || isalpha(b_expr[k-1]))
                         count--;
                 }
                 if (ob_count || prev_count == count)
                 {
                     printf("Invalid ) or content inside () missing\nTry Again\n");
                      return NULL;
                 }
                 b_expr[k] = '\0';
                 f->opr = mknode(b_expr, gram, flag);
            }
         }
     }
     if (strlen(expr) % 2 == 0)
     {
          printf("Illegal Expression\n");
          return NULL;
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
    root->visited = 0;
}

void search(struct semantic rule[], char a, struct node *root)
{
    int i;
    for (i = 0; i < 11; i++)
    {
        if (isdigit(a))
            a = 'd';
        else if (a > 96 && a < 122)
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
    }
}

void delete (struct node **root)
{
    if ((*root)->lchild)
    {
            delete(&(*root)->lchild);
    }
    if ((*root)->opr)
    {
            delete(&(*root)->opr);
    }
    if ((*root)->rchild)
    {
            delete(&(*root)->rchild);
    }

    free(*root);
    *root = NULL;
}
