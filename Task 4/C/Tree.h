#ifndef TREE_H
#define TREE_H
#include <stdlib.h> //malloc
#include "Queue.h"

typedef void create_line_func(void *);
typedef int compare(void *, void *);

typedef struct TreeNodeStruct
{
    void *element;
    struct TreeNodeStruct *left;
    struct TreeNodeStruct *right;
    struct TreeNodeStruct *parent;
} TreeNode;

TreeNode *new_tree_node(void *element, TreeNode *parent, TreeNode *left, TreeNode *right)
{
    TreeNode *res = (TreeNode *)(malloc(sizeof(TreeNode)));
    res->element = element;
    res->left = left;
    res->right = right;
    res->parent = parent;
    return res;
}

typedef struct
{
    TreeNode *root;
} Tree;

Tree *new_tree()
{
    Tree *res = (Tree *)(malloc(sizeof(Tree)));
    res->root = NULL;
    return res;
}

int find_height(TreeNode *node)
{
    if (!node) return -1;
    else {
        int lh = find_height(node->left);
        int rh = find_height(node->right);
        if (lh >= rh) return lh + 1;
        else return rh +1;
    }
}

int find_height_tree(Tree *tree)
{
    return find_height(tree->root);
}

void insert_node(Tree *tree, void *element, compare cmp)
{
    if (!tree->root)
    {
        tree->root = new_tree_node(element, NULL, NULL, NULL);
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
        parent->left = new_tree_node(element, parent, NULL, NULL);
    else
        parent->right = new_tree_node(element, parent, NULL, NULL);
}

void *level_order(Tree *tree, create_line_func handle_output)
{
    Queue *queue = new_queue(1<<find_height_tree(tree));
    add_to_queue(queue, tree->root);
    while (!empty_queue(queue))
    {
        TreeNode* curr = (TreeNode *)(next_in_queue(queue));
        handle_output(curr);
        if (curr->left)
            add_to_queue(queue, curr->left);
        if (curr->right)
            add_to_queue(queue, curr->right);
    }

    free(queue);
    return output;
}


#endif