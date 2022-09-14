#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct NodeStruct 
{
    int element;
    struct NodeStruct *next;
    struct NodeStruct *prev;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int size;
} DoubleLink;

typedef struct 
{
    Node *place;
} Iterator;

Node* new_node(int e, Node *n, Node* p) 
{
    Node *res = (Node*)(malloc(sizeof(Node)));
    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

void add_first_pos(DoubleLink *l, int value)
{
    Node* new = new_node(value, l->head, NULL);
    l->head = new;
    if (!l->tail) l->tail = new;
    else new->next->prev = new;
    l->size++;
}

void add_last_pos(DoubleLink *l, int value)
{
    Node* new = new_node(value, NULL, l->tail);
    if (l->tail) l->tail->next = new;
    else l->head = new;
    l->tail = new;
    l->size++;
}

Node* remove_node(DoubleLink *l, Node* n)
{
    if(n->prev)
        n->prev->next = n->next;
    else l->head= n->next;
    if(n->next)
        n->next->prev = n->prev;
    else l->tail = n->prev;
    n->next = NULL;
    n->prev = NULL;
    l->size--;
    return n;
}

Node* find_nr(DoubleLink *l, int nr)
{
    if(nr < l->size)
    {
        Node* this = l->head;
        for (int i = 0; i < nr; ++i) this = this->next;
        return this;
    }
    else return NULL;
}

void start(Iterator *iter, DoubleLink* l)
{
    iter->place = l->head;
}

int end(Iterator *iter)
{
    return !iter->place;
}

int findElement(Iterator *iter)
{
    if(!end(iter)) return iter->place->element;
    else return (int) HUGE_VAL;
}

void next(Iterator *iter)
{
    if(!end(iter))
        iter->place = iter->place->next;
}

void addition(DoubleLink *l, Node *a, int b) 
{
    a->element = a->element + b;
    if (a->element > 9)
    {
        if (a->prev == NULL) 
        {
            add_first_pos(l, 0);
        }
        addition(l, a->prev, 1);
        a->element = a->element - 10;
    }
}

void subtraction(DoubleLink *l, Node *a, int b) 
{
    a->element = a->element - b;
    if (a->element < 0)
    {
        a->element = a->element + 10;
        subtraction(l, a->prev, 1);
        if (l->head->element == 0)
        {
            remove_node(l, l->head);
            l->head = a;
        }
    }
}

void swap_lists(DoubleLink **l1, DoubleLink **l2)
{
    DoubleLink *temp = malloc(sizeof(DoubleLink));
    Iterator *iter_a = malloc(sizeof(Iterator))
    , *iter_b = malloc(sizeof(Iterator));

    int size_l1 = (*l1)->size + 1
    , size_l2 = (*l2)->size + 1
    , i;

    start(iter_a, *l2);
    start(iter_b, *l1);

    while(!end(iter_a))
    {
        add_last_pos(temp, iter_a->place->element);
        next(iter_a);
    }

    while (!end(iter_b))
    {
        add_last_pos(*l2, iter_b->place->element);
        next(iter_b);
    }

    start(iter_a, temp);
    while(!end(iter_a))
    {
        add_last_pos(*l1, iter_a->place->element);
        next(iter_a);
    }
    
    for (i = 0; i < size_l2; i++) 
    {
        remove_node(*l2, (*l2)->head);
    }
    for (i = 0; i < size_l1; i++) 
    {
        remove_node(*l1, (*l1)->head);
    }
    free(iter_a);
    free(iter_b);
    free(temp);
}

void arithmetic(DoubleLink *l1, DoubleLink *l2, void (func)(DoubleLink*, Node*, int))
{
    int i, place;
    Iterator *iter_a = malloc(sizeof(Iterator))
    , *iter_b = malloc(sizeof(Iterator));

    if (func == addition && l2->size > l1->size) 
    {
        swap_lists(&l1, &l2);
    }

    start(iter_a, l1);
    start(iter_b, l2);
    place = l1->size - l2->size;

    for (i = 0; i < place; i++)
    {
        next(iter_a);
    }

    while(!(end(iter_a) || end(iter_b))) 
    {
        func(l1, iter_a->place, iter_b->place->element);
        next(iter_a);
        next(iter_b);
    }

    free(iter_a);
    free(iter_b);
}

void printDoubleLink(DoubleLink *l1, DoubleLink *l2, void (func)(DoubleLink*, Node*, int))
{
    printf("Here is the answer\n");
    arithmetic(l1, l2, func);
    Node *cur = l1->head;
    while (cur != NULL) 
    {
        printf("%d", cur->element);
        cur = cur->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    /*
     * argv[1] = first number
     * argv[2] = operator (+ or -)
     * argv[3] = second number
     */
    void (*func)(DoubleLink*, Node*, int);
    if (argc != 4) 
    {
        fprintf(stderr, "wrong amount of input\n");
        return 1;
    }
    if (*argv[2] != '-' && *argv[2] != '+') 
    {
        fprintf(stderr, "This program only allows addition and subtraction.\n");
        return 1;
    } else if (*argv[2] == '-')
    {
        func = &subtraction;
    } else
    {
        func = &addition;
    }
    char c, c1, c2;
    c1 = argv[1][0];
    c2 = argv[3][0];
    DoubleLink *a = malloc(sizeof(DoubleLink)), 
    *b = malloc(sizeof(DoubleLink));
    Node *a_head = new_node((int)c1 - '0', NULL, NULL);
    Node *b_head = new_node((int)c2 - '0', NULL, NULL);
    a->head = a_head;
    a->tail = a_head;
    b->head = b_head;
    b->tail = b_head;

    while ((c = *(++argv[1])) != 0)
        add_last_pos(a, (int)c - '0');

    while ((c = *(++argv[3])) != 0)
        add_last_pos(b, (int)c - '0');
    
    printDoubleLink(a, b, func);
    free(a);
    free(b);
}