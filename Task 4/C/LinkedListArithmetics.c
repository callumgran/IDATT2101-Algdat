#include <stdlib.h>
#include <stdio.h>
#include "DoublyLinkedList.h"

typedef void func_type(DoublyLinkedList*, ListNode*, int);

void addition(DoublyLinkedList *l, ListNode *a, int b) 
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

void subtraction(DoublyLinkedList *l, ListNode *a, int b) 
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

void swap_lists_same_size(DoublyLinkedList **l1, DoublyLinkedList **l2, Iterator *iter_a, Iterator *iter_b)
{
    int c;
    start(iter_a, *l2);
    start(iter_b, *l1);
    while(!end(iter_a))
    {
        c = iter_a->place->element;
        iter_a->place->element = iter_b->place->element;
        iter_b->place->element = c;
        next(iter_a); next(iter_b);
    }
}

void swap_lists_diff_size(DoublyLinkedList **l1, DoublyLinkedList **l2, Iterator *iter_a, Iterator *iter_b)
{
    DoublyLinkedList *temp = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    Iterator *iter_c = (Iterator*)malloc(sizeof(Iterator));
    int l1_size = (*l1)->size, c;

    start(iter_a, *l2);
    while(!end(iter_a))
    {
        add_last_pos(temp, iter_a->place->element);
        next(iter_a);
    }

    start(iter_a, *l2);
    start(iter_b, *l1);
    start(iter_c, temp);
    while (!end(iter_c))
    {
        if (!end(iter_b))
        {
            iter_a->place->element = iter_b->place->element;
            iter_b->place->element = iter_c->place->element;
            next(iter_b);
        }
        else 
        {
            if ((*l2)->size > l1_size)
                remove_node(*l2, (*l2)->tail);
            add_last_pos(*l1, iter_c->place->element);
        }
        next(iter_a);
        next(iter_c);
    }

    free(iter_c);
    free(temp);
}

void arithmetic(DoublyLinkedList *l1, DoublyLinkedList *l2, func_type func)
{
    int i, place, sub_swapped = 1;
    Iterator *iter_a = (Iterator*)malloc(sizeof(Iterator))
    , *iter_b = (Iterator*)malloc(sizeof(Iterator));
    
    if (l2->size > l1->size) 
    {
        swap_lists_diff_size(&l1, &l2, iter_a, iter_b);
        if (func == &subtraction) sub_swapped = -1;
    }
    else if (l1->size == l2->size && l2->head->element > l1->head->element)
    {
        swap_lists_same_size(&l1, &l2, iter_a, iter_b);
        if (func == &subtraction) sub_swapped = -1;
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

    l1->head->element *= sub_swapped;

    free(iter_a);
    free(iter_b);
}

void clean_print_number(DoublyLinkedList **l)
{
    Iterator *iter = (Iterator*)malloc(sizeof(Iterator));
    start(iter, *l);
    while (iter->place->element == 0 && (*l)->size > 1)
    {
        next(iter);
        remove_node(*l, iter->place->prev);
        (*l)->head = iter->place;
    }
    start(iter, *l);
    while (!(end(iter)))
    {
        printf("%d", iter->place->element);
        next(iter);
    }
    free(iter);
}

void print_result(DoublyLinkedList *l1, DoublyLinkedList *l2, func_type func)
{
    clean_print_number(&l1);
    printf("%s", func == addition ? " + " : " - ");
    clean_print_number(&l2);
    arithmetic(l1, l2, func);
    printf(" = ");
    clean_print_number(&l1);
    printf("\n");
}

int main(int argc, char *argv[])
{
    /*
     * argv[1] = first number
     * argv[2] = operator (+ or -)
     * argv[3] = second number
     */
    void (*func)(DoublyLinkedList*, ListNode*, int);
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

    DoublyLinkedList *a = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList)), 
    *b = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));

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
    return 0;
}