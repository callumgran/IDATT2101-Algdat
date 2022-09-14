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

    if (l->tail)
        l->tail->next = new;
    else
        l->head= new;

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

int is_empty(DoubleLink *l)
{
    return l->head == NULL;
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

DoubleLink *addition2(DoubleLink *l1, DoubleLink *l2)
{
    DoubleLink *res = malloc(sizeof(DoubleLink));
    DoubleLink *l, *s;  // largest, smallest
    int value;
    int extraVal = 0;

    if (l1->size > l2->size) {
        l = l1;
        s = l2;
    } else {
        l = l2;
        s = l1;
    }
    do {
        value = s->tail->element + l->tail->element;
        add_first_pos(res, s->tail->element + l->tail->element + extraVal);
        if (res->head->element > 9)
        {
            res->head->element = res->head->element - 10;
            extraVal = 1;
        } else {
            extraVal = 0;
        }
        s->tail = s->tail->prev;
        l->tail = l->tail->prev;
    } while (s->tail != NULL && l->tail !=NULL);

    /* add last digits */
    while (l->tail != NULL) {
        add_first_pos(res, l->tail->element + extraVal);
        l->tail = l->tail->prev;
    }

    return res;
}

void printDoubleLink(DoubleLink *l)
{
    Node *cur = l->head;
    while (cur != NULL) {
        printf("%d", cur->element);
        cur = cur->next;
    }

    putchar('\n');
}

int main(int argc, char *argv[])
{
    /*
     * argv[1] = first number
     * argv[2] = operator (+ or -)
     * argv[3] = second number
     */
    if (argc != 4) {
        fprintf(stderr, "wrong amount of input");
        return 1;
    }

    char c, c1, c2;
    c1 = argv[1][0];
    c2 = argv[3][0];
    DoubleLink a, b;
    Node *a_head = new_node((int)c1 - '0', NULL, NULL);
    Node *b_head = new_node((int)c2 - '0', NULL, NULL);
    a.head = a_head;
    a.tail = a_head;
    b.head = b_head;
    b.tail = b_head;

    while ((c = *(++argv[1])) != 0)
        add_last_pos(&a, (int)c - '0');

    while ((c = *(++argv[3])) != 0)
        add_last_pos(&b, (int)c - '0');

    DoubleLink *result = addition2(&a, &b);
    printDoubleLink(result);
}
