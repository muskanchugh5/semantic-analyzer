#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct prod
{
    char left;
    char right[10];
    int end;
};

struct queue
{
    struct node *nodeptr;
    struct queue *next;
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

struct semanticRule
{
    char c;
    char *action;
    char *end;
};

void createlistbfs(struct node *);
int treeDFS(struct node *, struct semanticRule []);
void search(struct semanticRule [], char, struct node *);
void delete(struct node **);
void memberValueSet(struct node **, struct prod[], int, int);
void grammar(struct prod []);
void digitAlphaNode(int *, int *, int *, struct prod [], struct node **, struct node **, struct node **, struct node *, char);
void openCloseBracketNodes(struct prod gram[], struct node *f, struct node **ob, struct node **cb);
struct node * memoryAlloc();
struct node * bracketOperationCall (struct prod [], char [], int *, int *);
struct node *mknode(char *, struct prod [], int);

int main()
{
    struct prod gram[6] = {'E', "TP", 0, 
                           'P', "+TP|-TP", 1,
                           'T', "FQ", 0,
                           'Q', "*FQ|/FQ", 1,
                           'F', "d", 0,
                           'F', "(E)", 0};

    struct semanticRule rules[11] = {'d', "F.val = id.lexval","",
                                     'F', "Q.inh = F.val","",
                                     '*', "Q1.inh = Q.inh * F.val","",
                                     '/', "Q1.inh = Q.inh / F.val","",
                                     '+', "P1.inh = P.inh + T.val","",
                                     '-', "P1.inh = P.inh - T.val","",
                                     'T', "P.inh = T.val","",
                                     'Q', "Q.syn = Q.inh","T.val = Q.syn",
                                     'P', "P.syn = P.inh","E.val = P.syn",
                                     'E', "L.val = E.val","",
                                     ')', "F.val = E.val", ""};
    struct node *S = NULL;
    char expr[10] = "";
    int ch, choice = 0;

    do
    {
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
                        createlistbfs(S);
                    }
                    else
                        printf("No parse tree exist\n");
                    break;

            case 4: if (S)
                    {
                        printf("\n\nDisplaying Semantic Rule for each node\n");
                        treeDFS(S, rules);
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

struct node * memoryAlloc()
{
    return (struct node *)malloc(sizeof(struct node));
}

struct node* mknode(char *expr, struct prod gram[], int flag)
{
    struct node *t, *p, *f, *q, *id, *e, *temp, *ob, *cb;
    t = p = f = q = id = e = temp = ob = cb = NULL;
    int countOfOperands = 0;
    int digitFlag = 0;
    int operatorFlag = 0;
   
    e = memoryAlloc();
    e->c = gram[0].left;
    e->first = 0;
    if (flag == 0)
    {
        e->first = 1;
        flag = 1;
    }

    e->opr = e->rchild = e->lchild = NULL;
        
    p = memoryAlloc();
    p->c = gram[0].right[1];
    e->rchild = p;
    p->opr = p->lchild = p->rchild = NULL;
    p->first = 1;
        
    t = memoryAlloc();
    t->c = gram[0].right[0];
    e->lchild = t;
    t->opr = t->lchild = t->rchild = NULL;

    printf("%c -> %c%c\n", e->c, t->c, p->c);

    int i, j;
    char ch;
    for(i = 0; i < strlen(expr); i++)
    {
        if(isdigit(expr[i]) || isalpha(expr[i]))
        countOfOperands++;
    }
    i = 0;
    ch = expr[i++];
    if (ch == '(')
    {
        f = memoryAlloc();
        t->lchild = f;
        memberValueSet(&f, gram, 2, 0);

        q = memoryAlloc();
        t->rchild = q;
        q->first = 1;
        memberValueSet(&q, gram, 2, 1);

        printf("%c -> %c%c\n", t->c, f->c, q->c);
        openCloseBracketNodes(gram, f, &ob, &cb);
        printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);
        f->opr = bracketOperationCall(gram, expr, &i, &countOfOperands);
        ch = expr[i++];
    }         
    while (countOfOperands)
    {
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && !isdigit(ch) && !isalpha(ch))
        {
            printf("Invalid Character: %c\nTry Again\n", ch);
            return NULL;
        }
        if (isdigit(ch) || isalpha(ch))
        {
            digitAlphaNode(&digitFlag, &operatorFlag, &countOfOperands, gram, &f, &id, &q, t, ch);
            ch = expr[i++];
        }
        else if (ch == '+' || ch == '-')
        {
            if (!operatorFlag)
            {
                digitFlag = 0;
                operatorFlag = 1;
            }
            else if (operatorFlag)
            {
                printf("Invalid use of +/-\nTry Again\n");
                return NULL;
            }

            t = memoryAlloc();
            p->lchild = t;
            memberValueSet(&t, gram, 1, 1);

            id = memoryAlloc();
            p->opr = id;
            id->opr = id->lchild = id->rchild = NULL;
            id->c = ch;

            temp = p;
            p = memoryAlloc();
            temp->rchild = p;
            p->first = 0;
            memberValueSet(&p, gram, 1, 2);

            if (q->rchild == NULL && q->lchild == NULL && q->opr == NULL)
                  printf("%c -> ϵ\n", q->c);

            printf("%c -> %c%c%c\n", temp->c, id->c, t->c, p->c);

            ch = expr[i++];
            if (isdigit(ch) || isalpha(ch))
            {
                digitAlphaNode(&digitFlag, &operatorFlag, &countOfOperands, gram, &f, &id, &q, t, ch);
                ch = expr[i++];
            }
            else if (ch == ')')
            {
                printf("Illegal use of ) after + or -\nTry again\n");
                return NULL;
            }
            else if (ch == '(')
            {
                f = memoryAlloc();
                t->lchild = f;
                memberValueSet(&f, gram, 2, 0);

                q = memoryAlloc();
                t->rchild = q;
                q->first = 1;
                memberValueSet(&q, gram, 2, 1);
                printf("%c -> %c%c\n", t->c, f->c, q->c);

                openCloseBracketNodes(gram, f, &ob, &cb);

                printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                f->opr = bracketOperationCall(gram, expr, &i, &countOfOperands);
                ch = expr[i++];
            }
         }
         else if (ch == '*' || ch == '/')
         {
             if (!operatorFlag)
             {
                 digitFlag = 0;
                 operatorFlag = 1;
             }
             else if (operatorFlag)
             {
                 printf("Invalid */-\nTry Again\n");
                 return NULL;
             }
             f = memoryAlloc();
             q->lchild = f;
             memberValueSet(&f, gram, 3, 1);

             id = memoryAlloc();
             q->opr = id;
             id->c = ch;
             id->val = 0;
             id->rchild = id->lchild = id->opr = NULL;

             temp = q;
             q = memoryAlloc();
             q->first = 0;
             temp->rchild = q;
             memberValueSet(&q, gram, 3, 2);

             printf("%c -> %c%c%c\n", temp->c, id->c, f->c, q->c);

             ch = expr[i++];
             if (isdigit(ch) || isalpha(ch))
             {
                 /*case of digits and alphabets only*/
                 if (!digitFlag)
                 {
                     digitFlag = 1;
                     operatorFlag = 0;
                 }
                 else if (digitFlag)
                 {
                    printf("Use of multiple digit\nTry Again\n");
                    return NULL;
                 }
                 id = memoryAlloc();
                 f->opr = id;
                 id->c = ch;
                 id->val = ch - '0';
                 id->opr = id->lchild = id->rchild = NULL;

                 countOfOperands--;
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
                 openCloseBracketNodes(gram, f, &ob, &cb);

                 printf("%c -> %c%c%c\n", f->c, ob->c, 'E', cb->c);

                 f->opr = bracketOperationCall(gram, expr, &i, &countOfOperands);
                 ch = expr[i++];
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

void digitAlphaNode(int *digitFlag, int *operatorFlag, int *countOfOperands, struct prod gram[],
                    struct node **f, struct node **id, struct node **q, struct node *t,
                    char ch)
{
    if (!(*digitFlag))
    {
        (*digitFlag) = 1;
        (*operatorFlag) = 0;
    }
    else if ((*digitFlag))
    {
        printf("Use of multiple digit\nTry Again\n");
        return;
    }
    (*countOfOperands)--;
    *f = memoryAlloc();
    t->lchild = *f;
    memberValueSet(&(*f), gram, 2, 0);

    *id = memoryAlloc();
    (*f)->opr = *id;
    (*id)->opr = (*id)->lchild = (*id)->rchild = NULL;
    (*id)->c = ch;
    (*id)->val = ch - '0';

    (*q) = memoryAlloc();
    t->rchild = (*q);
    (*q)->first = 1;
    memberValueSet(&(*q), gram, 2, 1);

    printf("%c -> %c%c\n", t->c, (*f)->c, (*q)->c);
    printf("%c -> %c\n", (*f)->c, (*id)->c);
}

void openCloseBracketNodes(struct prod gram[], struct node *f, struct node **ob, struct node **cb)
{
    (*ob) = memoryAlloc();
    f->lchild = (*ob);
    memberValueSet(&(*ob), gram, 5, 0);

    (*cb) = memoryAlloc();
    f->rchild = (*cb);
    memberValueSet(&(*cb), gram, 5, 2);
}

struct node * bracketOperationCall (struct prod gram[], char expr[], int *i, int *countOfOperands)
{
    int j = *i, k = 0, ob_count = 0;
    char b_expr[20];
    int prev_count = *countOfOperands;

    while (expr[j] != ')' || ob_count)
    {
        b_expr[k++] = expr[j++];
        if(b_expr[k-1] == '(')
            ob_count++;
        else if(b_expr[k-1] == ')')
            ob_count--;
        else if(isdigit(b_expr[k-1]) || isalpha(b_expr[k-1]))
            (*countOfOperands)--;
    }
    if (ob_count || prev_count == *countOfOperands)
    {
         printf("Invalid ) or content inside () missing\nTry Again\n");
         return NULL;
    }
    b_expr[k] = '\0';
    (*i) = ++j;

    return mknode(b_expr, gram, 1);
}

void memberValueSet(struct node **tempNode, struct prod gram[], int m, int n)
{
    (*tempNode)->c = gram[m].right[n];
    (*tempNode)->rchild = (*tempNode)->lchild = (*tempNode)->opr = NULL;
    (*tempNode)->val = 0;
}

int treeDFS(struct node *root, struct semanticRule rules[])
{
    char *str;
    root->visited = 1;

    if (root->lchild)
        if(root->lchild->visited == 0)
            treeDFS(root->lchild, rules);

    if (root->opr)
        if(root->opr->visited == 0)
            treeDFS(root->opr, rules);

    if (root->rchild)
        if(root->rchild->visited == 0)
            treeDFS(root->rchild, rules);

    printf("[%c]\n", root->c);
    search(rules, root->c, root);
    root->visited = 0;
}

void search(struct semanticRule rules[], char a, struct node *root)
{
    int i;

    for (i = 0; i < 11; i++)
    {
        if (isdigit(a))
            a = 'd';
        else if (islower(a))
            a = 'd';
        if (rules[i].c == 'E' && !root->first)
            return;
        if (rules[i].c == a)
        {
            printf("%s\n", rules[i].action);
            if (root->first == 1)
                printf("%s\n", rules[i].end);
            printf("\n");
            return;
        }
    }
}

void createlistbfs(struct node *root)
{
    struct queue *qhead, *qrear, *qtemp, *qrelease;

    if (root == NULL)
    {
        return;
    }
    qhead = (struct queue *)malloc(sizeof(struct queue));
    qhead->nodeptr = root;
    qhead->next = NULL;
    qrear = qhead;
    while (qhead != NULL)
    {
        printf("%c  ", qhead->nodeptr->c);
        if (qhead->nodeptr->opr != NULL)
        {
            qtemp = (struct queue *)malloc(sizeof(struct queue));
            qtemp->nodeptr = qhead->nodeptr->opr;
            qtemp->next = NULL;
            qrear->next = qtemp;
            qrear = qtemp;
        }
        if (qhead->nodeptr->lchild != NULL)
        {
            qtemp = (struct queue *)malloc(sizeof(struct queue));
            qtemp->nodeptr = qhead->nodeptr->lchild;
            qtemp->next = NULL;
            qrear->next = qtemp;
            qrear = qtemp;
        }
        if (qhead->nodeptr->rchild != NULL)
        {
            qtemp = (struct queue *)malloc(sizeof(struct queue));
            qtemp->nodeptr = qhead->nodeptr->rchild;
            qtemp->next = NULL;
            qrear->next = qtemp;
            qrear = qtemp;
        }
        qrelease = qhead;
        qhead = qhead->next;
        free(qrelease);
    }
}

void delete (struct node **root)
{
    if ((*root)->lchild)
        delete(&(*root)->lchild);
    if ((*root)->opr)
        delete(&(*root)->opr);
    if ((*root)->rchild)
        delete(&(*root)->rchild);

    free(*root);
    *root = NULL;
}
