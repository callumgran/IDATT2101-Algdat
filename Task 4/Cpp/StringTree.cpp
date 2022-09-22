#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "Tree.h"
#include "DoublyLinkedList.h"
using namespace std;

// With line width 3 * 64 line width and 3 spaces.
// The tree still prints neatly with 6-7 levels.
#define LINE_WIDTH (1 * 64) // Width of each line
#define PRINT_HEIGHT 4 // Lines to print of the tree
#define SPACE_BETWEEN_LEVELS 1 // The space between levels on the tree

int compare_lists(void *element_1, void *element_2)
{
    int a, b, largest;
    Iterator *iter_a = new Iterator(),
            *iter_b = new Iterator();
    DoublyLinkedList
        *l1 = static_cast<DoublyLinkedList *>(element_1),
        *l2 = static_cast<DoublyLinkedList *>(element_2);

    if (l1->get_size() > l2->get_size())
    {
        largest = 1;
        iter_a->start(l1);
        iter_b->start(l2);
    }
    else if (l1->get_size() < l2->get_size())
    {
        largest = 2;
        iter_a->start(l2);
        iter_b->start(l1);
    }
    else
    {
        largest = 0;
        iter_a->start(l2);
        iter_b->start(l1);
    }

    while (!iter_b->end())
    {
        a = iter_a->get_place()->get_element();
        b = iter_b->get_place()->get_element();
        if (a < 0 && b < 0)
        {
            if (a > b)
                goto return_larger;
            if (a < b)
                goto return_smaller;
        }
        else if (a < 0)
            goto return_larger;
        else if (b < 0)
            goto return_smaller;
        if (a > b)
            goto return_larger;
        else if (a < b)
            goto return_smaller;
        iter_a->next();
        iter_b->next();
    }

    if (largest == 2)
        return -1;
    else if (largest == 1)
        return 1;
    else
        return 0;
return_larger:
    return largest == 1 && largest != 0 ? 1 : -1;
    
return_smaller:
    return largest == 1 && largest != 0 ? -1 : 1;
}

void add_white_space(DoublyLinkedList **out)
{
    int i, j;
    for (i = 0; i < PRINT_HEIGHT; i++)
    {
        for (j = 0; j < LINE_WIDTH; j++)
            (*out)->add_last_pos(' ');
        for (j = 0; j < SPACE_BETWEEN_LEVELS; j++)
            (*out)->add_last_pos('\n');
    }
}

void add_empty_nodes(void *element)
{
    TreeNode *curr = static_cast<TreeNode *>(element);
    DoublyLinkedList *empty_list = new DoublyLinkedList();
    if (curr->get_right() == NULL)
        curr->set_right(new TreeNode(empty_list, curr, NULL, NULL));
    if (curr->get_left() == NULL)
        curr->set_left(new TreeNode(empty_list, curr, NULL, NULL));
}

void print_tree(void *element)
{
    Iterator *iter = new Iterator();
    DoublyLinkedList *output = static_cast<DoublyLinkedList *>(element);
    if (output->get_tail()->get_element() != '\n') output->add_last_pos('\n');
    iter->start(output);
    while (!iter->end())
    {
        printf("%c", iter->get_place()->get_element());
        iter->next();
    }
}

void node_to_output(void *element)
{
    Iterator *iter_temp = new Iterator(),
             *iter_out = new Iterator();
    TreeNode *curr_node = static_cast<TreeNode *>(element);
    int i = 0, whitespace = 0;
    static int nodes, startplace, level;
    static DoublyLinkedList *out = new DoublyLinkedList();
    if (curr_node && level < PRINT_HEIGHT)
    {
        DoublyLinkedList *temp = static_cast<DoublyLinkedList *>(curr_node->get_element());
        if (level == 0) add_white_space(&out);
        if (temp != NULL)
        {
            if (temp->get_size() % 2 != 0)
            {
                i = 1;
            }

            whitespace = ((LINE_WIDTH) / pow(2, level + 1)) - temp->get_size() / 2;
            startplace += whitespace;

            iter_temp->start(temp);
            iter_out->start(out);
            while (i < startplace)
            {
                iter_out->next();
                i++;
            }
            while (!iter_temp->end())
            {
                iter_out->get_place()->set_element(iter_temp->get_place()->get_element());
                iter_out->next();
                iter_temp->next();
            }
            startplace += whitespace + temp->get_size() + (temp->get_size() % 2 != 0 ? -1 : 0);
            nodes++;
        }
        else
        {
            startplace += (LINE_WIDTH) / pow(2, level);
            nodes++;
        }

        if (nodes == pow(2, level))
        {
            nodes = 0;
            level += 1;
            if (startplace != level * (LINE_WIDTH + SPACE_BETWEEN_LEVELS))
                startplace = level * (LINE_WIDTH + SPACE_BETWEEN_LEVELS);
        }
    add_empty_nodes(curr_node);
    }
    else if (out != NULL) {
        print_tree(out);
        out = NULL;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "You must input atleast one word.\n");
        return 1;
    }
    Tree *tree = new Tree();
    DoublyLinkedList *a;
    int c;

    for (int i = 1; i < argc; i++)
    {
        a = new DoublyLinkedList();
        a->add_last_pos(*(argv[i]));
        while ((c = *(++argv[i])) != 0)
        {
            a->add_last_pos(c);
        }
        tree->insert_node(a, &compare_lists);
    }

    tree->level_order(&node_to_output);
    return 0;
}