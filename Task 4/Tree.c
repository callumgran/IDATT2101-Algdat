#include <stdlib.h>
#include <stdio.h>

typedef void func_type(void*);
typedef int key_type(void*);

typedef struct NodeStruct {
    void* element;
    struct NodeStruct* left;
    struct NodeStruct* right;
    struct NodeStruct* parent;
} Node;

typedef struct {
    Node* root;
} Tree;

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

int empty(Tree *tree)
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
        int lh = find_node_height(n->left); //Left height
        int rh = find_node_height(n->right); //Right height
        if (lh >= rh) return lh + 1;
        else return rh + 1;
    }
}

int find_tree_height(Tree *tree)
{
    return find_node_height(tree->root);
}

void level_order(Tree *tree, func_type print_node)
{

}

void insert_node(Tree *tree, void *element, key_type find_key)
{
    int key = find_key(element);
    Node *node = tree->root;
    if (!tree->root)
    {
        t->root = new_node(element, NULL, NULL, NULL);
        return;
    }
    int comp;
    Node *f = NULL;
    while (node)
    {
        f = node;
        comp = find_key(node->element);
        if (key < comp) node = node->left;
        else node = node->right;
    }
    if (key < comp) f->left = new_node(element, f, NULL, NULL);
    else f->right = new_node(element, f, NULL, NULL);
}

Node* find(Tree *tree, int key, key_type find_key)
{
    Node* node = tree->root;
    while (node)
    {
        int comp = find_key(n->element);
        if(key == comp) return node;
        else if (key < comp) node = node->left;
        else node = node->right;
    }
    return NULL;
}

void print_node(void *)
{
    // TODO: Write func.
}

int find_alphabetical_key(void *)
{
    // TODO: Write func.
}

int main()
{

    return 0;
}
