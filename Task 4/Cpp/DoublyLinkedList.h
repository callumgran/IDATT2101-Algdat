#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H
#include <stdlib.h>
#include <stdio.h>

class ListNode
{
    public:
        ListNode(int element, ListNode *prev, ListNode *next);
        ~ListNode();
        void set_element(int element);
        void set_prev(ListNode *prev);
        void set_next(ListNode *next);
        int get_element();
        ListNode *get_prev();
        ListNode *get_next();

    private:
        int element;
        ListNode *next, *prev;
};

ListNode::ListNode(int element, ListNode *next, ListNode *prev)
{
    this->element = element;
    this->next = next;
    this->prev = prev;
}

ListNode::~ListNode()
{
    delete this;
}

int ListNode::get_element()
{
    return this->element;
}

ListNode* ListNode::get_prev()
{
    return this->prev;
}

ListNode* ListNode::get_next()
{
    return this->next;
}

void ListNode::set_element(int element)
{
    this->element = element;
}

void ListNode::set_prev(ListNode *prev)
{
    this->prev = prev;
}

void ListNode::set_next(ListNode *next)
{
    this->next = next;
}

class DoublyLinkedList
{
    public:
        DoublyLinkedList();
        ~DoublyLinkedList();
        void add_last_pos(int value);
        void set_head(ListNode *head);
        void set_tail(ListNode *tail);
        void set_size(int size);
        ListNode *get_head();
        ListNode *get_tail();
        int get_size();
        void destroy_linked_list();
        void add_last_pos(void* value);

    private:
        void destroy_linked_list(ListNode *node);
        ListNode *head, *tail;
        int size;
};

DoublyLinkedList::DoublyLinkedList()
{
    this->head = NULL;
    this->tail = NULL;
    this->size = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
    destroy_linked_list();
}

void DoublyLinkedList::destroy_linked_list()
{
    destroy_linked_list(this->head);
}

void DoublyLinkedList::destroy_linked_list(ListNode *node)
{
    if(node!=NULL)
    {
        destroy_linked_list(node->get_next());
        delete node;
    }
}

ListNode* DoublyLinkedList::get_head()
{
    return this->head;
}

ListNode* DoublyLinkedList::get_tail()
{
    return this->tail;
}

int DoublyLinkedList::get_size()
{
    return this->size;
}

void DoublyLinkedList::set_head(ListNode *head)
{
    this->head = head;
}

void DoublyLinkedList::set_tail(ListNode *tail)
{
    this->tail = tail;
}

void DoublyLinkedList::set_size(int size)
{
    this->size = size;
}

void DoublyLinkedList::add_last_pos(int value)
{
    ListNode* node = new ListNode(value, NULL, this->tail);
    if (this->tail != NULL)
        this->tail->set_next(node);
    else
        this->set_head(node);
    this->set_tail(node);
    ++this->size;
}

class Iterator
{
    public:
        Iterator();
        ~Iterator();
        void start(DoublyLinkedList *l);
        void next();
        void prev();
        bool end();
        ListNode* get_place();

    private:
        ListNode *place;
};

Iterator::Iterator(){}

Iterator::~Iterator()
{
    delete this;
}

ListNode* Iterator::get_place()
{
    return this->place;
}

void Iterator::start(DoublyLinkedList *l)
{
    this->place = l->get_head();
}

bool Iterator::end()
{
    return this->place == NULL;
}

void Iterator::next()
{
    this->place = this->place->get_next();
}

void Iterator::prev()
{
    if (!this->end())
        this->place = this->place->get_prev();
}

#endif 