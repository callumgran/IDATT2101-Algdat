#ifndef TREE_H
#define TREE_H
#include <stdlib.h>
#include "Queue.h"

typedef void handle(void *);
typedef int compare(void *, void *);

class TreeNode
{
    public:
        TreeNode(void *element, TreeNode *parent, TreeNode *left, TreeNode *right);
        ~TreeNode();
        void set_left(TreeNode *left);
        void set_right(TreeNode *right);
        void set_parent(TreeNode *parent);
        void set_element(void *element);
        TreeNode *get_left();
        TreeNode *get_right();
        TreeNode *get_parent();
        void *get_element();

    private:
        void *element;
        TreeNode *parent, *left, *right;
};

TreeNode::TreeNode(void *element, TreeNode *parent, TreeNode *left, TreeNode *right)
{
    this->element = element;
    this->parent = parent;
    this->left = left;
    this->right = right;
}

TreeNode::~TreeNode()
{
    delete this;
}

void *TreeNode::get_element()
{
    return this->element;
}

TreeNode* TreeNode::get_left()
{
    return this->left;
}

TreeNode* TreeNode::get_right()
{
    return this->right;
}

TreeNode* TreeNode::get_parent()
{
    return this->parent;
}

void TreeNode::set_element(void *element)
{
    this->element = element;
}

void TreeNode::set_left(TreeNode *left)
{
    this->left = left;
}

void TreeNode::set_right(TreeNode *right)
{
    this->right = right;
}

void TreeNode::set_parent(TreeNode* parent)
{
    this->parent = parent;
}

class Tree
{
    public:
        Tree();
        ~Tree();

        void insert_node(void *element, compare cmp);
        void level_order(handle handle);
        void destroy_tree();
        int find_height();
        void set_root(TreeNode* root);
        TreeNode* get_root();
    
    private:
        TreeNode *root;
        void destroy_tree(TreeNode *);
        int find_height(TreeNode *node);
};

Tree::Tree()
{
    this->root = NULL;
}

void Tree::insert_node(void *element, compare cmp)
{
    if (!this->root)
    {
        TreeNode *new_node = new TreeNode(element, NULL, NULL, NULL);
        this->set_root(new_node);
        return;
    }

    TreeNode *node = this->get_root();
    TreeNode *parent = NULL;
    
    while (node != NULL)
    {
        parent = node;
        if (cmp(element, parent->get_element()) < 1)
            node = node->get_left();
        else
            node = node->get_right();
    }
    TreeNode *new_node = new TreeNode(element, parent, NULL, NULL);
    if (cmp(element, parent->get_element()) < 1)
        parent->set_left(new_node);
    else 
        parent->set_right(new_node);
}

int Tree::find_height()
{
    return find_height(this->root);
}

void Tree::level_order(handle handle)
{
    Queue *queue = new Queue(1<<find_height());
    queue->add_to_queue(this->root);
    while (!queue->empty_queue())
    {
        TreeNode *curr = static_cast<TreeNode *>(queue->next_in_queue());
        handle(curr);
        if (curr->get_left() != NULL)
            queue->add_to_queue(curr->get_left());
        if (curr->get_right() != NULL)
            queue->add_to_queue(curr->get_right());
    }
}

Tree::~Tree()
{
    destroy_tree();
}

void Tree::destroy_tree()
{
    destroy_tree(this->root);
}

void Tree::destroy_tree(TreeNode *node)
{
    if(node!=NULL)
    {
        destroy_tree(node->get_left());
        destroy_tree(node->get_right());
        delete node;
    }
}

int Tree::find_height(TreeNode *node)
{
    if (node == NULL) return -1;
    else {
        int lh = find_height(node->get_left());
        int rh = find_height(node->get_right());
        if (lh >= rh) return lh + 1;
        else return rh +1;
    }
}

TreeNode* Tree::get_root()
{
    return this->root;
}

void Tree::set_root(TreeNode* root)
{
    this->root = root;
}

#endif