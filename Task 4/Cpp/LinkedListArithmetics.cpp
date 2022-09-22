#include <stdlib.h>
#include <stdio.h>
#include "CppDoublyLinkedList.h"
using namespace std;

typedef void func_type(DoublyLinkedList*, ListNode*, int);

void addition(DoublyLinkedList *l, ListNode *a, int b) 
{
    a->set_element(a->get_element() + b);
    if (a->get_element() > 9)
    {
        if (a->get_prev() == NULL) 
        {
            l->add_first_pos(0);
        }
        addition(l, a->get_prev(), 1);
        a->set_element(a->get_element() - 10);
    }
}

void subtraction(DoublyLinkedList *l, ListNode *a, int b) 
{
    a->set_element(a->get_element() - b);
    if (a->get_element() < 0)
    {
        a->set_element(a->get_element() + 10);
        subtraction(l, a->get_prev(), 1);
        if (l->get_head()->get_element() == 0)
        {
            l->remove_node(l->get_head());
            l->set_head(a);
        }
    }
}

void swap_lists_same_size(DoublyLinkedList **l1, DoublyLinkedList **l2, Iterator *iter_a, Iterator *iter_b)
{
    int c;
    iter_a->start(*l2);
    iter_b->start(*l1);
    while(!iter_a->end())
    {
        c = iter_a->get_place()->get_element();
        iter_a->get_place()->set_element(iter_b->get_place()->get_element());
        iter_b->get_place()->set_element(c);
        iter_a->next(); iter_b->next();
    }
}

void swap_lists_diff_size(DoublyLinkedList **l1, DoublyLinkedList **l2, Iterator *iter_a, Iterator *iter_b)
{
    DoublyLinkedList *temp = new DoublyLinkedList();
    Iterator *iter_c = new Iterator();
    int l1_size = (*l1)->get_size(), c;

    iter_a->start(*l2);
    while(!iter_a->end())
    {
        temp->add_last_pos(iter_a->get_place()->get_element());
        iter_a->next();
    }

    iter_a->start(*l2);
    iter_b->start(*l1);
    iter_c->start(temp);
    while (!iter_c->end())
    {
        if (!iter_b->end())
        {
            iter_a->get_place()->set_element(iter_b->get_place()->get_element());
            iter_b->get_place()->set_element(iter_c->get_place()->get_element());
            iter_b->next();
        }
        else 
        {
            if ((*l2)->get_size() > l1_size)
                (*l2)->remove_node((*l2)->get_tail());
            (*l1)->add_last_pos(iter_c->get_place()->get_element());
        }
        iter_a->next();
        iter_c->next();
    }

    free(iter_c);
    free(temp);
}

void arithmetic(DoublyLinkedList *l1, DoublyLinkedList *l2, func_type func)
{
    int i, 
        place, 
        sub_swapped = 1,
        l1_size = l1->get_size(),
        l2_size = l2->get_size();
    Iterator *iter_a = new Iterator(),
            *iter_b = new Iterator();

    if (l2_size > l1_size) 
    {
        swap_lists_diff_size(&l1, &l2, iter_a, iter_b);
        if (func == &subtraction) sub_swapped = -1;
    }
    else if (l2_size == l1_size && l2->get_head()->get_element() > l1->get_head()->get_element())
    {
        swap_lists_same_size(&l1, &l2, iter_a, iter_b);
        if (func == &subtraction) sub_swapped = -1;
    }

    iter_a->start(l1);
    iter_b->start(l2);
    place = l1->get_size() - l2->get_size();

    for (i = 0; i < place; i++)
    {
        iter_a->next();
    }

    while(!(iter_a->end() || iter_b->end()))
    {
        func(l1, iter_a->get_place(), iter_b->get_place()->get_element());
        iter_a->next();
        iter_b->next();
    }

    l1->get_head()->set_element(l1->get_head()->get_element() * sub_swapped);

    free(iter_a);
    free(iter_b);
}

void clean_print_number(DoublyLinkedList **l)
{
    Iterator *iter = new Iterator();
    iter->start(*l);
    while (iter->get_place()->get_element() == 0 && (*l)->get_size() > 1)
    {
        iter->next();
        (*l)->remove_node(iter->get_place()->get_prev());
        (*l)->set_head(iter->get_place());
    }
    iter->start(*l);
    while (!iter->end())
    {
        printf("%d", iter->get_place()->get_element());
        iter->next();
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

    DoublyLinkedList *a = new DoublyLinkedList(), 
                    *b = new DoublyLinkedList();

    char c;
    a->add_last_pos((int)*(argv[1]) - '0');
    while ((c = *(++argv[1])) != 0)
        a->add_last_pos((int)c - '0');

    b->add_last_pos((int)*(argv[3]) - '0');
    while ((c = *(++argv[3])) != 0)
        b->add_last_pos((int)c - '0');
    
    print_result(a, b, func);
    free(a);
    free(b);
    return 0;
}