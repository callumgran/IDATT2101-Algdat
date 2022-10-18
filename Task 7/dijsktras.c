#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define swap(a, b) { struct vertex_t c = a; a = b; b = c; }

struct heap_t {
    int len;
    int capacity;
    struct vertex_t *nodes;
};

struct prev_t {
    int dist;
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

static inline int heap_left_child(struct heap_t *heap, int i)
{
    heap->nodes[left(i)].d->dist;
}

static inline int heap_right_child(struct heap_t *heap, int i)
{
    heap->nodes[right(i)].d->dist;
}

static inline int heap_parent(struct heap_t *heap, int i)
{
    heap->nodes[over(i)].d->dist;
}

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

int peek(struct heap_t *heap)
{
    if (heap->len == 0)
        return INFINITY;
    return heap->nodes[0].d->dist;
}

void heapify_up(struct heap_t *heap)
{
    int i = heap->len - 1;
    while(has_parent(i, heap->len) && heap_parent(heap, i) > heap->nodes[i].d->dist) {
        swap(heap->nodes[over(i)], heap->nodes[i]);
        i = over(i);
    }
}

void heapify_down(struct heap_t *heap)
{
    int i = 0;
    while(has_left(i, heap->len)) {
        int smaller = left(i);
        if (has_right(i, heap->len) && heap_right_child(heap, i) < heap_left_child(heap, i))
            smaller = right(i);
        if (heap->nodes[i].d->dist < heap->nodes[smaller].d->dist)
            break;
        else
            swap(heap->nodes[i], heap->nodes[smaller]);
        i = smaller;
    }
}

struct vertex_t *heap_pull(struct heap_t *heap)
{
    struct vertex_t *item = (struct vertex_t *)(malloc(sizeof(struct vertex_t)));
    *item = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->len]; 
    heapify_down(heap);
    return item;
}

void heap_push(struct heap_t *heap, struct vertex_t *vertex)
{
    heap->nodes[heap->len] = *vertex;
    heap->len++;
    heapify_up(heap);
}

void fix_heap(struct heap_t *heap, int i)
{
    int m = left(i);
    if (m < heap->len) {
        int h = m + 1;
        if (h < heap->len && heap->nodes[h].d->dist < heap->nodes[m].d->dist) 
            m = h;
        if (heap->nodes[m].d->dist < heap->nodes[i].d->dist) {
            swap(heap->nodes[i], heap->nodes[m]);
            fix_heap(heap, m);
        }
    }
}

struct prev_t *malloc_prev()
{
    struct prev_t *prev = (struct prev_t *)(malloc(sizeof(struct prev_t)));
    prev->dist = INFINITY;
    return prev;
}

void init_prev(struct graph_t *graph, struct vertex_t *vertex)
{
    int i;
    for (i = graph->vertices; i--;)
        graph->v_list[i].d = malloc_prev();
    vertex->d->dist = 0;
}

struct graph_t *malloc_graph(FILE *file)
{
    int i;
    struct graph_t *graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    fscanf(file, "%d %d\n", &graph->vertices, &graph->edges);
    graph->v_list = (struct vertex_t*)(calloc(graph->vertices, sizeof(struct vertex_t)));
    for (i = 0; i < graph->vertices; i++) {
        graph->v_list[i].node_index = i; 
    }
    for (i = 0; i  < graph->edges; i++) {
        int from,
            to;
        struct edge_t *edge = (struct edge_t*)(malloc(sizeof(struct edge_t)));
        fscanf(file, "%d %d %d\n", &from, &to, &edge->weight);
        edge->to_vertex = &graph->v_list[to];
        edge->next_edge = graph->v_list[from].first_edge;
        graph->v_list[from].first_edge = edge;
    }
    return graph;
}

void shorten(struct vertex_t *vertex, struct edge_t *edge)
{
    struct prev_t *prev = vertex->d, 
                *prev_next = edge->to_vertex->d;
    if (prev_next->dist > prev->dist + edge->weight) {
        prev_next->dist = prev->dist + edge->weight;
        prev_next->prev = vertex;
    }
}

void dijkstra(struct graph_t *graph, struct vertex_t *vertex)
{
    int i;
    init_prev(graph, vertex);
    struct heap_t *prio_q = malloc_heap(graph->vertices);
    for (i = 0; i < graph->vertices; i++)
        heap_push(prio_q, &graph->v_list[i]);
    for (i = graph->vertices; i > 1; --i) {
        struct vertex_t *next = heap_pull(prio_q);
        for (struct edge_t *edge = next->first_edge; edge; edge = edge->next_edge) {
            shorten(next, edge);
        }
    }
    free_heap(prio_q);
}

void find_shortest_path(char* file)
{
    FILE *textfile;
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
    dijkstra(g, &g->v_list[START_NODE]);
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
    printf("\n==============================================\n");
    free(g->v_list);
    free(g);
}

int main()
{
    char* file_1 = "vg1.txt";
    char* file_2 = "vg2.txt";
    char* file_3 = "vg3.txt";
    char* file_4 = "vg4.txt";
    char* file_5 = "vg5.txt";
    // char* file_6 = "vgSkandinavia.txt";
    find_shortest_path(file_1);
    find_shortest_path(file_2);
    find_shortest_path(file_3);
    find_shortest_path(file_4);
    find_shortest_path(file_5);
    // find_shortest_path(file_6);
    return 0;
}
