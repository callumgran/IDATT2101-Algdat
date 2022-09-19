#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define LINE_WIDTH 128

typedef void print_func(void *);
typedef void create_line(void *, void *, int);
typedef int key_type(void *);
typedef int line_size(int);
typedef int compare(void *, void *);

typedef struct NodeStruct
{
    void *element;
    struct NodeStruct *left;
    struct NodeStruct *right;
    struct NodeStruct *parent;
} Node;

typedef struct
{
    Node *root;
} Tree;

typedef struct
{
    void **elements;
    int start, end, size, max;
} Queue;

typedef struct CharNode
{
    char element;
    struct CharNode *next;
    struct CharNode *prev;
} CharNode;

typedef struct
{
    CharNode *head;
    CharNode *tail;
    int size;
} DoublyLinked;

typedef struct
{
    CharNode *place;
} Iterator;

CharNode *new_char_node(char e, CharNode *n, CharNode *p)
{
    CharNode *res = (CharNode *)(malloc(sizeof(CharNode)));

    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

void add_first_pos(DoublyLinked *l, char value)
{
    CharNode *new = new_char_node(value, l->head, NULL);
    l->head = new;

    if (!l->tail)
        l->tail = new;
    else
        new->next->prev = new;
    l->size++;
}

void add_last_pos(DoublyLinked *l, char value)
{
    CharNode *new = new_char_node(value, NULL, l->tail);
    
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

Node *new_node(void *element, Node *parent, Node *left, Node *right)
{
    Node *res = (Node *)(malloc(sizeof(Node)));
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

int find_depth(Node *node)
{
    int d = -1;
    while (node)
    {
        d++;
        node = node->parent;
    }
    return d;
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
        if (iter_a->place->element < 0)
            goto return_larger;
        if (iter_b->place->element < 0)
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

    if (largest == 2)
        return -1;
    else if (largest == 1)
        return 1;
    else
        return 0;
}

void insert_node(Tree *tree, void *element, compare cmp)
{
    if (!tree->root)
    {
        tree->root = new_node(element, NULL, NULL, NULL);
        return;
    }

    Node *node = tree->root;
    Node *parent = NULL;

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

void *check_queue(Queue *queue)
{
    if (empty_queue(queue))
        return NULL;
    return queue->elements[queue->start];
}

void level_order(Tree *tree, create_line handle_output, print_func print_func)
{
    int prev_size = 0, level = 0;
    Queue *queue = new_queue(10);
    DoublyLinked *output = (DoublyLinked *)(malloc(sizeof(DoublyLinked)));
    add_to_queue(queue, tree->root);
    while (!empty_queue(queue))
    {
        Node *this = (Node *)(next_in_queue(queue));
        if (this)
        {
            if (find_depth(this) == 4)
                break;
            if (find_depth(this) == level)
            {
                handle_output(this->element, &output, find_depth(this));
                if (output->size - prev_size >= LINE_WIDTH)
                {
                    add_last_pos(output, (char)'\n');
                    prev_size = output->size;
                }
            }
            else
            {
                level++;
                while (output->size - prev_size <= LINE_WIDTH)
                {
                    add_last_pos(output, (char)' ');
                }
                add_last_pos(output, (char)'\n');
                prev_size = output->size;
                handle_output(this->element, &output, find_depth(this));
            }

            add_to_queue(queue, this->left);
            add_to_queue(queue, this->right);
        }
    }
    
    free(queue);
    print_func(output);
}

void create_out(void *element, void *output, int level)
{
    Iterator *iter = (Iterator *)(malloc(sizeof(Iterator)));
    DoublyLinked *temp = (DoublyLinked *)element, 
        **out = (DoublyLinked **)output;
    int i = 0,
        whitespace = (LINE_WIDTH - temp->size) / pow(2, level + 1);

    if (whitespace % 2 != 0)
    {
        i = 1;
        add_first_pos(temp, (char)' ');
    }

    while (i < whitespace)
    {
        add_first_pos(temp, (char)' ');
        add_last_pos(temp, (char)' ');
        i++;
    }

    start(iter, temp);
    while (!end(iter))
    {
        add_last_pos(*out, (char)iter->place->element);
        next(iter);
    }

    free(iter);
}

void print_tree(void *element)
{
    Iterator *iter = (Iterator *)(malloc(sizeof(Iterator)));
    DoublyLinked *temp = (DoublyLinked *)element;

    start(iter, temp);
    while (!end(iter))
    {
        printf("%c", (char)iter->place->element);
        next(iter);
    }

    printf("\n");
    free(iter);
}

void print_result(Tree *tree)
{
    level_order(tree, &create_out, &print_tree);
    printf("\n");
}

int main(int argc, char *argv[])
{
    Tree *tree = new_tree();
    DoublyLinked *a = (DoublyLinked *)malloc(sizeof(DoublyLinked));
    char c;

    for (int i = 1; i < argc; i++)
    {
        a = (DoublyLinked *)malloc(sizeof(DoublyLinked));
        add_last_pos(a, (char)*(argv[i]));
        while ((c = *(++argv[i])) != 0)
        {
            add_last_pos(a, (char)c);
        }
        insert_node(tree, a, &compare_lists);
    }

    print_result(tree);

    return 0;
}