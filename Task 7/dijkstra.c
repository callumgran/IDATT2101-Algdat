#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct heap_t heap_t;

typedef struct prev_t prev_t;

typedef struct edge_t edge_t;

typedef struct vertex_t vertex_t;

typedef struct graph_t graph_t;

#define START_NODE 1
#define INFINITY 999999999
#define left(i) (i << 1) + 1
#define right(i) (i + 1) << 1
#define over(i) (i - 1) >> 1
#define has_left(i, size) (left(i) < size)
#define has_right(i, size) (right(i) < size)
#define has_parent(i, size) (over(i) >= 0)
#define distance(heap, i) (heap->nodes[i].d->dist)
#define swap(a, b) {                    \
        struct vertex_t c = a;          \
        int h_i_a = a.d->heap_index;    \
        int h_i_b = b.d->heap_index;    \
        a = b;                          \
        b = c;                          \
        a.d->heap_index = h_i_a;        \
        b.d->heap_index = h_i_b;        \
    }                                   \

struct heap_t {
    int len;
    int capacity;
    struct vertex_t *nodes;
};

struct prev_t {
    int dist;
    int heap_index;
    struct vertex_t *prev;
};

struct edge_t {
    struct edge_t *next_edge;
    struct vertex_t *to_vertex;
    int weight;
};

struct vertex_t {
    int node_index;
    struct edge_t *first_edge;
    struct prev_t *d;
};

struct graph_t {
    struct vertex_t *v_list;
    int vertices;
    int edges;
};

struct heap_t *malloc_heap(int capacity)
{
    struct heap_t *heap = (struct heap_t *)(malloc(sizeof(struct heap_t)));
    heap->capacity = capacity;
    heap->len = 0;
    heap->nodes = (struct vertex_t *)(malloc(capacity * sizeof(struct vertex_t)));
}

void free_heap(struct heap_t *heap)
{
    free(heap->nodes);
    free(heap);
}

void heapify_up(struct heap_t *heap, int i)
{
    while(has_parent(i, heap->len) && distance(heap, over(i)) > distance(heap, i)) {
        swap(heap->nodes[over(i)], heap->nodes[i]);
        i = over(i);
    }
}

void heapify_down(struct heap_t *heap)
{
    int i = 0;
    while(has_left(i, heap->len)) {
        int smaller = left(i);
        if (has_right(i, heap->len) && distance(heap, right(i)) < distance(heap, smaller))
            smaller = right(i);
        if (distance(heap, i) < distance(heap, smaller))
            break;
        else {
            swap(heap->nodes[i], heap->nodes[smaller]);
            i = smaller;
        }
    }
}

struct prev_t *malloc_prev(int i)
{
    struct prev_t *prev = (struct prev_t *)(malloc(sizeof(struct prev_t)));
    prev->dist = INFINITY;
    prev->heap_index = i;
    if (i == START_NODE)
        prev->dist = 0;
    return prev;
}

struct graph_t *malloc_graph(FILE *file)
{
    int i, ret;
    struct graph_t *graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    ret = fscanf(file, "%d %d\n", &graph->vertices, &graph->edges);
    graph->v_list = (struct vertex_t*)(calloc(graph->vertices, sizeof(struct vertex_t)));
    for (i = 0; i < graph->vertices; i++) {
        graph->v_list[i].node_index = i; 
    }
    for (i = 0; i < graph->edges; i++) {
        int from,
            to;
        struct edge_t *edge = (struct edge_t*)(malloc(sizeof(struct edge_t*)));
        ret = fscanf(file, "%d %d %d\n", &from, &to, &edge->weight);
        edge->to_vertex = &graph->v_list[to];
        edge->next_edge = graph->v_list[from].first_edge;
        graph->v_list[from].first_edge = edge;
    }
    return graph;
}

void free_graph(struct graph_t *graph)
{
    free(graph->v_list);
    free(graph);
}

void dijkstra(struct graph_t *graph, int len)
{
    struct heap_t *prio_q = malloc_heap(len);
    for (int i = 0; i < len; i++) {
        graph->v_list[i].d = malloc_prev(i);
        prio_q->nodes[prio_q->len] = graph->v_list[i];
        prio_q->len++;
        heapify_up(prio_q, i);
    }
    while (prio_q->len) {
        int change = 0;
        struct vertex_t *next = (struct vertex_t *)(malloc(sizeof(struct vertex_t*)));
        *next = prio_q->nodes[0];
        prio_q->nodes[0] = prio_q->nodes[--prio_q->len];
        heapify_down(prio_q);
        for (struct edge_t *edge = next->first_edge; edge; edge = edge->next_edge) {
            if (edge->to_vertex->d->dist > next->d->dist + edge->weight) {
                edge->to_vertex->d->dist = next->d->dist + edge->weight;
                edge->to_vertex->d->prev = next;
                heapify_up(prio_q, edge->to_vertex->d->heap_index);
                change = 1;
            }
        }
        if (change == 1)
            heapify_down(prio_q);
    }
}


void find_shortest_path(char* file)
{
    FILE *textfile;
    clock_t t;
    double elapsed;
    textfile = fopen(file, "r");
    if (textfile == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }
    struct graph_t *g = malloc_graph(textfile); 
    fclose(textfile);
    printf("\n==============================================");
    printf("\nFile: %s", file);
    printf("\nTotal Edges \tTotal Nodes");
    printf("\n\t%d\t\t%d\n", g->edges, g->vertices);
    t = clock();
    dijkstra(g, g->vertices);
    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    if (file == "vg5.txt" || file == "vg1.txt") {
        printf("\nNode: %d", START_NODE);
        printf("\nNode \tPrev \tDistance");
        for (int i = 0; i < g->vertices; i++) {
            putchar('\n');
            if (g->v_list[i].d->dist == INFINITY)
                printf("%d\t \tunvisited ",  g->v_list[i].node_index);
            else if (g->v_list[i].node_index == g->v_list[START_NODE].node_index)
                printf("%d\tstart\t0",  g->v_list[i].node_index);
            else {
                printf("%d\t%d\t%d", g->v_list[i].node_index, g->v_list[i].d->prev->node_index, g->v_list[i].d->dist);
            }
        }
        putchar('\n');
    }
    printf("Time used: %f ms \n", time_taken);
    printf("\n==============================================\n");
    free_graph(g);
}

int main()
{
    char* file_1 = "vg1.txt";
    char* file_2 = "vg2.txt";
    char* file_3 = "vg3.txt";
    char* file_4 = "vg4.txt";
    char* file_5 = "vg5.txt";
    find_shortest_path(file_1);
    find_shortest_path(file_2);
    find_shortest_path(file_3);
    find_shortest_path(file_4);
    find_shortest_path(file_5);
    
    return 0;
}