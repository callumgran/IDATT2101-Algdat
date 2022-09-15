#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef void print_func(void*, int);
typedef int key_type(void*);
typedef int line_size(int);
typedef int compare(void* , void*);

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

CharNode* new_char_node(char e, CharNode *n, CharNode* p) 
{
    CharNode *res = (CharNode*)(malloc(sizeof(CharNode)));
    res->element = e;
    res->next = n;
    res->prev = p;
    return res;
}

void add_first_pos(DoublyLinked *l, char value)
{
    CharNode* new = new_char_node(value, l->head, NULL);
    l->head = new;
    if (!l->tail) l->tail = new;
    else new->next->prev = new;
    l->size++;
}

void add_last_pos(DoublyLinked *l, char value)
{
    CharNode* new = new_char_node(value, NULL, l->tail);
    if (l->tail) l->tail->next = new;
    else l->head = new;
    l->tail = new;
    l->size++;
}

CharNode* remove_node(DoublyLinked *l, CharNode* n)
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

void start(Iterator *iter, DoublyLinked* l)
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

Node* new_node(void *element, Node *left, Node *right, Node *parent)
{
    Node *res = (Node*)(malloc(sizeof(Node)));
    res->element = element;
    res->left = left;
    res->right = right;
    res->parent = parent;
    return res;
}

Tree* new_tree()
{
    Tree *res = (Tree*)(malloc(sizeof(Tree)));
    res->root = NULL;
    return res;
}

int empty_tree(Tree *tree)
{
    return !tree->root;
}

int find_depth(Node *node)
{
    int d = -1; //Depth
    while (node)
    {
        d++;
        node = node->parent;
    }
    return d;
}

int find_node_height(Node *node)
{
    if(!node) return -1;
    else 
    {
        int lh = find_node_height(node->left); //Left height
        int rh = find_node_height(node->right); //Right height
        if (lh >= rh) return lh + 1;
        else return rh + 1;
    }
}

int find_tree_height(Tree *tree)
{
    return find_node_height(tree->root);
}

int compare_lists(void* element_1, void* element_2)
{
    Iterator 
    *iter_a = (Iterator*)malloc(sizeof(Iterator)),
    *iter_b = (Iterator*)malloc(sizeof(Iterator));
    DoublyLinked 
    *l1 = (DoublyLinked*)element_1,
    *l2 = (DoublyLinked*)element_2;

    int largest;
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
        if (iter_a->place->element > iter_b->place->element) return largest == 1 && largest != 0 ? 1 : -1;
        else if (iter_a->place->element < iter_b->place->element) return largest == 1 && largest != 0 ? -1 : 1;
        next(iter_a); 
        next(iter_b);
    }
    if (largest == 2) return -1;
    else if (largest == 1) return 1;
    else return 0;
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
        if (cmp(node->element, parent->element) <= 0) node = node->left;
        else node = node->right;
    }
    if (cmp(node->element, parent->element) <= 0) parent->left = new_node(element, parent, NULL, NULL);
    else parent->right = new_node(element, parent, NULL, NULL);
}

Node* find(Tree *tree, int key, key_type find_key)
{
    Node* node = tree->root;
    while (node)
    {
        int comp = find_key(node->element);
        if(key == comp) return node;
        else if (key < comp) node = node->left;
        else node = node->right;
    }
    return NULL;
}

Queue* new_queue(int size)
{
    Queue *queue = (Queue*)(malloc(sizeof(Queue)));
    queue->elements = (void*)(malloc(size*sizeof(void*)));
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
    if(full_queue(queue)) return;
    queue->elements[queue->end] = element;
    queue->end = (queue->end + 1)%queue->max;
    ++queue->size;
}

void* next_in_queue(Queue *queue)
{
    void *element;
    if(empty_queue(queue)) return NULL;
    element = queue->elements[queue->start];
    queue->start = (queue->start + 1)%queue->max;
    --queue->size;
    return element;
}

void* check_queue(Queue *queue)
{
    if(empty_queue(queue)) return NULL;
    return queue->elements[queue->start];
}

void level_order(Tree *tree, print_func print_func, line_size line_size)
{
    int line, node_height;
    Queue *queue = new_queue(10);
    add_to_queue(queue, tree->root);
    while (!empty_queue(queue))
    {
        Node *this = (Node*)(next_in_queue(queue));
        if (this)
        {
            node_height = find_node_height(this);
            print_func(this, node_height);
            line += line_size(node_height);
            if (line >= 64) 
            {
                printf("\n");
                line = 0;
            }
            add_to_queue(queue, this->left);
            add_to_queue(queue, this->right);
        }
    }
    free(queue);
}

int find_curr_line_size(int level)
{
    return 64/(int)pow(2, level);
}

void print_node(void *element, int level)
{
    int i = 0;
    Iterator *iter = (Iterator*)malloc(sizeof(Iterator));
    DoublyLinked *temp = (DoublyLinked *) element;
    int width = find_curr_line_size(level);
    int whitespace = (width - temp->size)/2;
    if (whitespace%2 != 0) {
        i = 1;
        add_first_pos(temp, (char)' ');
    }
    for (i < whitespace; i++;)
    {
        add_first_pos(temp, (char)' ');
        add_last_pos(temp, (char)' ');
    }
    start(iter, temp);
    while (!end(iter)) 
    {
        printf("%c", iter->place->element);
        next(iter);
    }
    free(iter);
}

int main(int argc, char *argv[])
{
    return 0;
}