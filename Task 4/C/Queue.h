#ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
    void **elements;
    int start, end, size, max;
} Queue;

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

#endif