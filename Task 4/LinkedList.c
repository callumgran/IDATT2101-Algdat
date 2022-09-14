#include <stdlib.h>
#include <stdio.h>

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

Node new_node(int e, Node *n, Node* p) 
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

Node* remove(DoubleLink *l, Node* n)
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
        for (int = 0; i < nr; ++i) this = this->next;
        return this;
    }
    else return null;
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

int is_empty(DoubleLink *l)
{
    return l->head == NULL;
}

void start(Iterator *iter)
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
    else return HUGE_VAL;
}

void next(Iterator *iter)
{
    if(!end(iter))
        iter->place = iter->place->next;
}