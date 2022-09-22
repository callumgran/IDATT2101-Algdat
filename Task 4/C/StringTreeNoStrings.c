#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

// With line width 3 * 64 line width and 3 spaces.
// The tree still prints neatly with 6-7 levels.
#define LINE_WIDTH (1 * 64) // Width of each line
#define PRINT_HEIGHT 4 // Lines to print of the tree
#define SPACE_BETWEEN_LEVELS 1 // The space between levels on the tree
#define QUEUE_SIZE (1<<PRINT_HEIGHT)

typedef void create_line_func(void *, void *);
typedef void add_white_spaces(void *);
typedef int compare(void *, void *);

typedef struct NodeStruct
{
    void *element;
    struct NodeStruct *left;
    struct NodeStruct *right;
    struct NodeStruct *parent;
} TreeNode;

typedef struct
{
    TreeNode *root;
} Tree;

typedef struct
{
    void **elements;
    int start, end, size, max;
} Queue;

typedef struct ListNode
{
    int element;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;
    int size;
} DoublyLinked;

typedef struct
{
    ListNode *place;
} Iterator;

ListNode *new_char_node(int e, ListNode *n, ListNode *p)
{
    ListNode *res = (ListNode *)(malloc(sizeof(ListNode)));

    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

void add_last_pos(DoublyLinked *l, int value)
{
    ListNode *new = new_char_node(value, NULL, l->tail);

    if (l->tail)
        l->tail->next = new;
    else
        l->head = new;
    l->tail = new;
    l->size++;
}

void start(Iterator *iter, DoublyLinked *l)
{
    iter->place = l->head;
}

int end(Iterator *iter)
{
    return !iter->place;
}

void next(Iterator *iter)
{
    if (!end(iter))
        iter->place = iter->place->next;
}

Queue *new_queue(int size)
{
    Queue *queue = (Queue *)(malloc(sizeof(Queue)));
    queue->elements = (void *)(malloc(size * sizeof(void *)));
    queue->max = size;
    queue->start = queue->end = queue->size = 0;
    return queue;
}

int empty_queue(Queue *queue)
{
    return !queue->size;
}

int full_queue(Queue *queue)
{
    return queue->size == queue->max;
}

void add_to_queue(Queue *queue, void *element)
{
    if (full_queue(queue))
        return;
    queue->elements[queue->end] = element;
    queue->end = (queue->end + 1) % queue->max;
    ++queue->size;
}

void *next_in_queue(Queue *queue)
{
    void *element;
    if (empty_queue(queue))
        return NULL;
    element = queue->elements[queue->start];
    queue->start = (queue->start + 1) % queue->max;
    --queue->size;
    return element;
}

TreeNode *new_node(void *element, TreeNode *parent, TreeNode *left, TreeNode *right)
{
    TreeNode *res = (TreeNode *)(malloc(sizeof(TreeNode)));
    res->element = element;
    res->left = left;
    res->right = right;
    res->parent = parent;
    return res;
}

Tree *new_tree()
{
    Tree *res = (Tree *)(malloc(sizeof(Tree)));
    res->root = NULL;
    return res;
}

void insert_node(Tree *tree, void *element, compare cmp)
{
    if (!tree->root)
    {
        tree->root = new_node(element, NULL, NULL, NULL);
        return;
    }

    TreeNode *node = tree->root;
    TreeNode *parent = NULL;

    while (node)
    {
        parent = node;
        if (cmp(element, parent->element) <= 0)
            node = node->left;
        else
            node = node->right;
    }

    if (cmp(element, parent->element) <= 0)
        parent->left = new_node(element, parent, NULL, NULL);
    else
        parent->right = new_node(element, parent, NULL, NULL);
}

void *level_order(Tree *tree, create_line_func handle_output, add_white_spaces init_output)
{
    Queue *queue = new_queue(QUEUE_SIZE);
    void *output = (void *)(malloc(sizeof(void)));
    init_output(&output);
    add_to_queue(queue, tree->root);
    while (!empty_queue(queue))
    {
        TreeNode *this = (TreeNode *)(next_in_queue(queue));
        handle_output(this, &output);
        if (this->left)
            add_to_queue(queue, this->left);
        if (this->right)
            add_to_queue(queue, this->right);
    }

    free(queue);
    return output;
}

int compare_lists(void *element_1, void *element_2)
{
    int a, b, largest;
    Iterator
        *iter_a = (Iterator *)malloc(sizeof(Iterator)),
        *iter_b = (Iterator *)malloc(sizeof(Iterator));
    DoublyLinked
        *l1 = (DoublyLinked *)element_1,
        *l2 = (DoublyLinked *)element_2;

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
        printf("\nLargest: %d", largest);
        printf("\nLargest list: %c", iter_a->place->element);
        printf("\nSmallest list: %c", iter_b->place->element);
        if (iter_a->place->element < 0 && iter_b->place->element < 0) // Ikke ascii karakterer, typ ÆØÅ
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
    printf("\n%c is before %c\n", b, a);
    return largest == 1 && largest != 0 ? 1 : -1;

return_smaller:
    free(iter_a);
    free(iter_b);
    printf("\n%c is before %c\n", a, b);
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

void add_white_space(void *output)
{
    DoublyLinked **out = (DoublyLinked **)output;
    int i, j;
    for (i = 0; i < PRINT_HEIGHT; i++)
    {
        for (j = 0; j < LINE_WIDTH; j++)
            add_last_pos(*out, ' ');
        for (j = 0; j < SPACE_BETWEEN_LEVELS; j++)
            add_last_pos(*out, '\n');
    }
}

void add_empty_nodes(void *element)
{
    TreeNode *this = (TreeNode *)element;
    DoublyLinked *empty_list = NULL;
    if (!this->right)
        this->right = new_node(empty_list, this, NULL, NULL);
    if (!this->left)
        this->left = new_node(empty_list, this, NULL, NULL);
}

void node_to_output(void *element, void *output)
{
    Iterator *iter_temp = (Iterator *)(malloc(sizeof(Iterator))),
             *iter_out = (Iterator *)(malloc(sizeof(Iterator)));
    TreeNode *this = (TreeNode *)element;
    int i = 0, whitespace = 0;
    static int nodes, startplace, level;
    if (this && level < PRINT_HEIGHT)
    {
        DoublyLinked *temp = (DoublyLinked *)this->element,
                     **out = (DoublyLinked **)output;

        if (temp)
        {
            if (temp->size % 2 != 0)
            {
                i = 1;
            }

            whitespace = ((LINE_WIDTH) / pow(2, level + 1)) - temp->size / 2;
            startplace += whitespace;

            start(iter_temp, temp);
            start(iter_out, *out);
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
    }
}

void print_tree(void *element)
{
    Iterator *iter = (Iterator *)(malloc(sizeof(Iterator)));
    DoublyLinked *output = (DoublyLinked *)element;
    if (output->tail->element != '\n') add_last_pos(output, '\n');
    start(iter, output);
    while (!end(iter))
    {
        printf("%c", iter->place->element);
        next(iter);
    }

    free(iter);
    free(element);
}

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "You must input atleast one word.\n");
        return 1;
    }
    Tree *tree = new_tree();
    DoublyLinked *a;
    int c;

    for (int i = 1; i < argc; i++)
    {
        a = (DoublyLinked *)malloc(sizeof(DoublyLinked));
        add_last_pos(a, *(argv[i]));
        while ((c = *(++argv[i])) != 0)
        {
            add_last_pos(a, c);
        }
        insert_node(tree, a, &compare_lists);
    }

    print_tree(level_order(tree, &node_to_output, &add_white_space));
    free(a);
    free(tree);
    return 0;
}