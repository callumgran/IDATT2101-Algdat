#ifndef CPPQUEUE_H
#define CPPQUEUE_H
#include <stdlib.h>

class Queue 
{
    public:
        Queue(int max);
        ~Queue();
        bool empty_queue();
        bool full_queue();
        void add_to_queue(void *element);
        void *next_in_queue();
        int get_size();

    private:
        void **elements;
        int start, end, size, max;
};

Queue::~Queue()
{
    delete this;
}

Queue::Queue(int max)
{
    this->elements = (void **)(malloc(max * sizeof(void *)));
    this->max = max;
    this->start = this->end = this->size = 0;
}

int Queue::get_size()
{
    return this->size;
}

bool Queue::empty_queue()
{
    return !this->size;
}

bool Queue::full_queue()
{
    return this->size == this->max;
}

void Queue::add_to_queue(void *element)
{
    if (this->full_queue())
        return;
    this->elements[this->end] = element;
    this->end = (this->end + 1) % this->max;
    ++this->size;
}

void* Queue::next_in_queue()
{
    void *element;
    if (empty_queue()) return NULL;
    element = this->elements[this->start];
    this->start = (this->start + 1) % this->max;
    --this->size;
    return element;
}

#endif