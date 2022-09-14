#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define replace_node(el21, el2) 

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
} DoublyLinked;

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

void add_first_pos(DoublyLinked *l, int value)
{
    Node* new = new_node(value, l->head, NULL);
    l->head = new;
    if (!l->tail) l->tail = new;
    else new->next->prev = new;
    l->size++;
}

void add_last_pos(DoublyLinked *l, int value)
{
    Node* new = new_node(value, NULL, l->tail);
    if (l->tail) l->tail->next = new;
    else l->head = new;
    l->tail = new;
    l->size++;
}

Node* remove_node(DoublyLinked *l, Node* n)
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

Node* find_nr(DoublyLinked *l, int nr)
{
    if(nr < l->size)
    {
        Node* this = l->head;
        for (int i = 0; i < nr; ++i) this = this->next;
        return this;
    }
    else return NULL;
}

void start(Iterator *iter, DoublyLinked* l)
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

void addition(DoublyLinked *l, Node *a, int b) 
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

void subtraction(DoublyLinked *l, Node *a, int b) 
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

void swap_lists(DoublyLinked **l1, DoublyLinked **l2, Iterator *iter_a, Iterator *iter_b)
{
    DoublyLinked *temp = malloc(sizeof(DoublyLinked));

    int size_l1 = (*l1)->size + 1
    , size_l2 = (*l2)->size + 1
    , i;

    start(iter_a, *l2);
    while(!end(iter_a))
    {
        add_last_pos(temp, iter_a->place->element);
        next(iter_a);
    }

    start(iter_a, *l2);
    start(iter_b, *l1);
    while (!end(iter_a))
    {
        if (!end(iter_b))
        {
            iter_a->place->element = iter_b->place->element;
            next(iter_b);
        }
        else remove_node(*l2, iter_a->place);
        next(iter_a);
    }

    start(iter_a, temp);
    start(iter_b, *l1);
    while (!end(iter_a))
    {
        if (!end(iter_b))
        {
            iter_b->place->element = iter_a->place->element;
            next(iter_b);
        }
        else add_last_pos(*l1, iter_a->place->element);
        next(iter_a);
    }

    free(temp);
}

void arithmetic(DoublyLinked *l1, DoublyLinked *l2, void (func)(DoublyLinked*, Node*, int))
{
    int i, place, sub_swapped = 0;
    Iterator *iter_a = malloc(sizeof(Iterator))
    , *iter_b = malloc(sizeof(Iterator));

    if (l2->size > l1->size || (l1->size == l2->size && l2->head->element > l1->head->element)) 
    {
        swap_lists(&l1, &l2, iter_a, iter_b);
        if (func == subtraction) sub_swapped = 1;
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

    if (sub_swapped) l1->head->element = l1->head->element * -1;

    free(iter_a);
    free(iter_b);
}


void print_number(DoublyLinked *l)
{
    Iterator *iter = malloc(sizeof(Iterator));
    start(iter, l);
    while (!(end(iter)))
    {
        printf("%d", iter->place->element);
        next(iter);
    }
    free(iter);
}

void print_result(DoublyLinked *l1, DoublyLinked *l2, void (func)(DoublyLinked*, Node*, int))
{
    print_number(l1);
    printf("%s", func == addition ? " + " : " - ");
    print_number(l2);
    arithmetic(l1, l2, func);
    printf(" = ");
    print_number(l1);
    printf("\n");
}

int main(int argc, char *argv[])
{
    /*
     * argv[1] = first number
     * argv[2] = operator (+ or -)
     * argv[3] = second number
     */
    void (*func)(DoublyLinked*, Node*, int);
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

    DoublyLinked *a = malloc(sizeof(DoublyLinked)), 
    *b = malloc(sizeof(DoublyLinked));

    char c;
    add_last_pos(a, (int)*(argv[1]) - '0');
    while ((c = *(++argv[1])) != 0)
        add_last_pos(a, (int)c - '0');

    add_last_pos(b, (int)*(argv[3]) - '0');
    while ((c = *(++argv[3])) != 0)
        add_last_pos(b, (int)c - '0');
    
    print_result(a, b, func);
    free(a);
    free(b);
}