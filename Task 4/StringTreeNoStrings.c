#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define LINE_WIDTH 128

typedef void create_line_func(void *, void *, int *);
typedef void add_white_spaces(void *, int);
typedef int key_type(void *);
typedef int line_size(int);
typedef int compare(void *, void *);
void print_tree(void *);

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

ListNode *new_char_node(char e, ListNode *n, ListNode *p)
{
    ListNode *res = (ListNode *)(malloc(sizeof(ListNode)));

    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

void add_first_pos(DoublyLinked *l, char value)
{
    ListNode *new = new_char_node(value, l->head, NULL);
    l->head = new;

    if (!l->tail)
        l->tail = new;
    else
        new->next->prev = new;
    l->size++;
}

void add_last_pos(DoublyLinked *l, char value)
{
    ListNode *new = new_char_node(value, NULL, l->tail);

    if (l->tail)
        l->tail->next = new;
    else
        l->head = new;
    l->tail = new;
    l->size++;
}

int empty_list(DoublyLinked *l)
{
    return !l->size;
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

int find_depth(TreeNode *node)
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

int find_height(TreeNode *node)
{
    if (!node)
        return -1;
    else
    {
        int left_height = find_height(node->left);
        int right_height = find_height(node->right);
        if (left_height >= right_height)
            return left_height + 1;
        return right_height + 1;
    }
}

void *level_order(Tree *tree, create_line_func handle_output, add_white_spaces init_output)
{
    int level = 0, height = find_height(tree->root);
    Queue *queue = new_queue(10);
    void *output = (void *)(malloc(sizeof(void)));
    init_output(&output, height);
    add_to_queue(queue, tree->root);
    while (!empty_queue(queue))
    {
        TreeNode *this = (TreeNode *)(next_in_queue(queue));
        handle_output(this->element, &output, &level);
        if (level < height)
        {
            DoublyLinked* empty_list = NULL;
            if (!this->right)
                this->right = new_node(empty_list, this, NULL, NULL);
            if (!this->left)
                this->left = new_node(empty_list, this, NULL, NULL);
            
        }
        if (this->left)
            add_to_queue(queue, this->left);
        if (this->right)
            add_to_queue(queue, this->right);
    }

    free(queue);
    return output;
}

void add_white_space(void *output, int tree_height)
{
    DoublyLinked **out = (DoublyLinked **)output;
    for (int i = 0; i <= tree_height; i++)
    {
        for (int j = 0; j < LINE_WIDTH; j++)
            add_last_pos(*out, ' ');
        add_last_pos(*out, '\n');
    }
}

void node_to_output(void *element, void *output, int *level)
{
    Iterator *iter_temp = (Iterator *)(malloc(sizeof(Iterator))),
        *iter_out = (Iterator *)(malloc(sizeof(Iterator)));
    DoublyLinked *temp = (DoublyLinked *)element,
        **out = (DoublyLinked **)output;
    int i = 0, whitespace = 0, r_level = *level + 1;
    static int nodes, startplace = 0;
    if (temp)
    {
        if (temp->size % 2 != 0)
        {
            i = 1;
        }
        whitespace = ((LINE_WIDTH)/pow(2, *level + 1)) - temp->size/2;
        startplace += whitespace;
        if (whitespace % 2 != 0)
        {
            whitespace += 1;
        }
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
        startplace = startplace + whitespace + temp->size;
    }
    else
    {
        startplace += (LINE_WIDTH)/pow(2, *level);
    }
    nodes++;
    if (nodes == pow(2, *level))
    {
        if (startplace != r_level * LINE_WIDTH)
            startplace = r_level * LINE_WIDTH + r_level;
        nodes = 0;
        *level += 1;
        startplace += 1;
    }
}

void print_tree(void *element)
{
    Iterator *iter = (Iterator *)(malloc(sizeof(Iterator)));
    DoublyLinked *output = (DoublyLinked *)element;

    start(iter, output);
    while (!end(iter))
    {
        printf("%c", iter->place->element);
        next(iter);
    }

    printf("\n");
    free(iter);
    free(element);
}

void print_result(Tree *tree)
{
    print_tree(level_order(tree, &node_to_output, &add_white_space));
    printf("\n");
}

int main(int argc, char *argv[])
{
    Tree *tree = new_tree();
    DoublyLinked *a = (DoublyLinked *)malloc(sizeof(DoublyLinked));
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

    print_result(tree);

    return 0;
}