#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

typedef struct ListNodeStruct 
{
    int element;
    struct ListNodeStruct *next;
    struct ListNodeStruct *prev;
} ListNode;

ListNode* new_list_node(int e, ListNode *n, ListNode* p) 
{
    ListNode *res = (ListNode*)(malloc(sizeof(ListNode)));
    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

typedef struct
{
    ListNode *head;
    ListNode *tail;
    int size;
} DoublyLinkedList;

void add_first_pos(DoublyLinkedList *l, int value)
{
    ListNode* new = new_list_node(value, l->head, NULL);
    l->head = new;
    if (!l->tail) l->tail = new;
    else new->next->prev = new;
    l->size++;
}

void add_last_pos(DoublyLinkedList *l, int value)
{
    ListNode* new = new_list_node(value, NULL, l->tail);
    if (l->tail) l->tail->next = new;
    else l->head = new;
    l->tail = new;
    l->size++;
}

ListNode* remove_node(DoublyLinkedList *l, ListNode* n)
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

typedef struct 
{
    ListNode *place;
} Iterator;

void start(Iterator *iter, DoublyLinkedList* l)
{
    iter->place = l->head;
}

int end(Iterator *iter)
{
    return !iter->place;
}

void next(Iterator *iter)
{
    if(!end(iter))
        iter->place = iter->place->next;
}

#endif