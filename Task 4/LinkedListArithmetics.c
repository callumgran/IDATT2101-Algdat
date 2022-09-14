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

void delete_all(DoubleLink *l)
{
    if(l->head)
    {
        Node* this = l->head;
        do {
            l->head = this->next;
            free(this);
            this = l->head;
        } while(this->next);
        free(this);
        l->head = NULL;
        l->size = 0;
    }
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

void addition_list(DoubleLink *l1, DoubleLink *l2)
{
    int i;
    int place = l1->size - l2->size;
    Iterator *iter_a = malloc(sizeof(Iterator))
    , *iter_b = malloc(sizeof(Iterator));
    start(iter_a, l1);
    start(iter_b, l2);
    for (i = 0; i < place; i++)
    {
        next(iter_a);
    }

    while(!(end(iter_a) || end(iter_b))) 
    {
        addition(l1, iter_a->place, iter_b->place->element);
        next(iter_a);
        next(iter_b);
    }
    free(iter_a);
    free(iter_b);
}

void subtraction(DoubleLink *l, Node *a, int b) 
{
    a->element = a->element - b;
    if (a->element < 1)
    {
        if (a->prev == NULL && a->prev == l->head) 
        {
            remove_node(l, l->head);
        } else {
            addition(l, a, 10);
            a->element = a->prev->element - 1;
        }
    }
}

void subtraction_list(DoubleLink *l1, DoubleLink *l2)
{
    int i;
    int place = l1->size - l2->size;
    Iterator *iter_a = malloc(sizeof(Iterator))
    , *iter_b = malloc(sizeof(Iterator));
    start(iter_a, l1);
    start(iter_b, l2);
    for (i = 0; i < place; i++)
    {
        next(iter_a);
    }

    while(!(end(iter_a) || end(iter_b))) 
    {
        subtraction(l1, iter_a->place, iter_b->place->element);
        next(iter_a);
        next(iter_b);
    }
    free(iter_a);
    free(iter_b);
}

void printDoubleLink(DoubleLink *l1, DoubleLink *l2,  void (func)(DoubleLink*, DoubleLink*))
{
    printf("Here is the answer\n");
    func(l1, l2);
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
    if (argc != 4) 
    {
        fprintf(stderr, "wrong amount of input\n");
        return 1;
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
    
    printDoubleLink(a, b, &subtraction_list);
    free(a);
    free(b);
}