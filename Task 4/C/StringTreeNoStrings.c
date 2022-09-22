#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "Tree.h"
#include "DoublyLinkedList.h"

// With line width 3 * 64 line width and 3 spaces.
// The tree still prints neatly with 6-7 levels.
#define LINE_WIDTH (1 * 64) // Width of each line
#define PRINT_HEIGHT 4 // Lines to print of the tree
#define SPACE_BETWEEN_LEVELS 1 // The space between levels on the tree
#define QUEUE_SIZE (1<<PRINT_HEIGHT)

int compare_lists(void *element_1, void *element_2)
{
    int a, b, largest;
    Iterator
        *iter_a = (Iterator *)malloc(sizeof(Iterator)),
        *iter_b = (Iterator *)malloc(sizeof(Iterator));
    DoublyLinkedList
        *l1 = (DoublyLinkedList *)element_1,
        *l2 = (DoublyLinkedList *)element_2;

    if (l1->size > l2->size)
    {
        largest = 1;
        start(iter_a, l1);
        start(iter_b, l2);
    }
    else if (l1->size < l2->size)
    {
        largest = 2;
        start(iter_a, l2);
        start(iter_b, l1);
    }
    else
    {
        largest = 0;
        start(iter_a, l2);
        start(iter_b, l1);
    }

    while (!end(iter_b))
    {
        if (iter_a->place->element < 0 && iter_b->place->element < 0)
        {
            if (iter_a->place->element < iter_b->place->element)
                goto return_larger;
            if (iter_a->place->element < iter_b->place->element)
                goto return_smaller;
        }
        else if (iter_a->place->element < 0)
            goto return_larger;
        else if (iter_b->place->element < 0)
            goto return_smaller;
        a = tolower(iter_a->place->element);
        b = tolower(iter_b->place->element);
        if (a > b)
            goto return_larger;
        else if (a < b)
            goto return_smaller;
        next(iter_a);
        next(iter_b);
    }

return_larger:
    free(iter_a);
    free(iter_b);
    return largest == 1 && largest != 0 ? 1 : -1;

return_smaller:
    free(iter_a);
    free(iter_b);
    return largest == 1 && largest != 0 ? -1 : 1;

    free(iter_a);
    free(iter_b);
    if (largest == 2)
        return -1;
    else if (largest == 1)
        return 1;
    else
        return 0;
}

void add_white_space(DoublyLinkedList **out)
{
    int i, j;
    for (i = 0; i < PRINT_HEIGHT; i++)
    {
        for (j = 0; j < LINE_WIDTH; j++)
            add_last_pos(*out, ' ');
        for (j = 0; j < SPACE_BETWEEN_LEVELS; j++)
            add_last_pos(*out, '\n');
    }
}

void add_empty_nodes(TreeNode *element)
{
    DoublyLinkedList *empty_list = NULL;
    if (!element->right)
        element->right = new_tree_node(empty_list, element, NULL, NULL);
    if (!element->left)
        element->left = new_tree_node(empty_list, element, NULL, NULL);
}

void print_output(DoublyLinkedList *output)
{
    Iterator *iter = (Iterator *)(malloc(sizeof(Iterator)));
    if (output->tail->element != '\n') add_last_pos(output, '\n');
    start(iter, output);
    while (!end(iter))
    {
        printf("%c", iter->place->element);
        next(iter);
    }

    free(iter);
    free(output);
}

void node_to_output(void *element)
{
    Iterator *iter_temp = (Iterator *)(malloc(sizeof(Iterator))),
             *iter_out = (Iterator *)(malloc(sizeof(Iterator)));
    TreeNode *this = (TreeNode *)element;
    int i = 0, whitespace = 0;
    static int nodes, startplace, level;
    static DoublyLinkedList *out;
    if(level == 0) {
        out = (DoublyLinkedList *)(malloc(sizeof(DoublyLinkedList)));
        add_white_space(&out);
    }
    if (this && level < PRINT_HEIGHT)
    {
        DoublyLinkedList *temp = (DoublyLinkedList *)this->element;
        if (temp)
        {
            if (temp->size % 2 != 0)
            {
                i = 1;
            }

            whitespace = ((LINE_WIDTH) / pow(2, level + 1)) - temp->size / 2;
            startplace += whitespace;

            start(iter_temp, temp);
            start(iter_out, out);
            while (i < startplace)
            {
                next(iter_out);
                i++;
            }
            while (!end(iter_temp))
            {
                iter_out->place->element = iter_temp->place->element;
                next(iter_out);
                next(iter_temp);
            }
            free(iter_out);
            free(iter_temp);
            startplace += whitespace + temp->size + (temp->size % 2 != 0 ? -1 : 0);
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

    add_empty_nodes(this);
    } else if (out) {
        print_output(out);
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
    Tree *tree = new_tree();
    DoublyLinkedList *a;
    int c;

    for (int i = 1; i < argc; i++)
    {
        a = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
        add_last_pos(a, *(argv[i]));
        while ((c = *(++argv[i])) != 0)
        {
            add_last_pos(a, c);
        }
        insert_node(tree, a, &compare_lists);
    }

    level_order(tree, &node_to_output);
    free(a);
    free(tree);
    return 0;
}