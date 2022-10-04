#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH (100)
#define INF 1000000000
#define swap(a, b) {int tmp = a; a = b; b = tmp;}

typedef struct edge_t edge_t;

typedef struct vertex_t vertex_t;

typedef struct graph_t graph_t;

void search_dfs(struct vertex_t *vertex, int choice);

struct edge_t {
    struct edge_t *next_edge;
    struct vertex_t *to_vertex;
};

struct vertex_t {
    size_t node_index;
    struct edge_t *edge;
    int visited;
    void *d;
}; 

struct graph_t {
    size_t  vertices, 
            edges;
    struct vertex_t *v_list;
};

typedef struct dfs_t {
    size_t  dist,
            found_time,
            end_time;
    struct vertex_t *prev;
} dfs_t;

struct graph_t *malloc_graph(FILE *file)
{
    size_t i;
    struct graph_t *graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    fscanf(file, "%zu %zu\n", &graph->vertices, &graph->edges);
    graph->v_list = (struct vertex_t*)(calloc(graph->vertices, sizeof(struct vertex_t)));
    for (i = 0; i < graph->vertices; i++) {
        graph->v_list[i].node_index = i; 
    }

    for (i = 0; i  < graph->edges; i++) {
        size_t  from,
                to;
        struct edge_t *edge = (struct edge_t*)(malloc(sizeof(struct edge_t)));
        fscanf(file, "%zu %zu\n", &from, &to);
        edge->to_vertex = &graph->v_list[to];
        edge->next_edge = graph->v_list[from].edge;
        graph->v_list[from].edge = edge;
    }
    return graph;
}

struct dfs_t *malloc_dfs()
{
    struct dfs_t *f = (struct dfs_t*)(calloc(1, sizeof(struct dfs_t)));
    f->dist = INF;
    return f;
}

struct graph_t *graph_transpose(struct graph_t *graph)
{
    size_t i, j;
    struct graph_t *t_graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    t_graph->v_list = (struct vertex_t*)(calloc(graph->vertices, sizeof(struct vertex_t)));
    t_graph->vertices = graph->vertices;
    t_graph->edges = graph->edges;
    for (i = 0; i < t_graph->vertices; i++) {
        t_graph->v_list[i].node_index = graph->v_list[i].node_index;
    }

    for(i = 0; i < t_graph->vertices; i++) {
        struct vertex_t *curr = &graph->v_list[i];
        for (struct edge_t *edge = curr->edge; edge; edge = edge->next_edge) {
            struct edge_t *new_edge = (struct edge_t*)(malloc(sizeof(struct edge_t)));
            new_edge->to_vertex = &t_graph->v_list[i];
            new_edge->next_edge = t_graph->v_list[edge->to_vertex->node_index].edge;
            t_graph->v_list[edge->to_vertex->node_index].edge = new_edge;
        }
    }

    return t_graph;
}

void init_dfs(struct graph_t *graph)
{
    for (size_t i = graph->vertices; i--;) {
        graph->v_list[i].d = malloc_dfs();
    }
    search_dfs(0, 0);
}

void search_dfs(struct vertex_t *vertex, int print)
{
    static int component;
    static size_t time;
    if (vertex) {
        if (!vertex->visited) {
            if (print)
                printf("%zu ", vertex->node_index);
            vertex->visited = 1;
            component = 1;
        }
        struct dfs_t *nd = (struct dfs_t*)vertex->d;
        nd->found_time = ++time;
        for (struct edge_t *edge = vertex->edge; edge; edge = edge->next_edge) {
            struct dfs_t *md = (struct dfs_t*)edge->to_vertex->d;
            if (!md->found_time) {
                md->prev = vertex;
                md->dist = nd->dist + 1;
                search_dfs(edge->to_vertex, print);
            }
        }
        nd->end_time = ++time;
    } else {
        time = 0;
        if (component) {
            printf("\n");
        }
        component = 0;
    }
}

void dfs(struct graph_t *graph, struct vertex_t *start, int print)
{
    init_dfs(graph);
    ((struct dfs_t*)start->d)->dist = 0;
    search_dfs(start, print);
}

size_t *bubble_sort(graph_t *graph)
{
    size_t i, j, 
        *new_order = (size_t *)(malloc(graph->vertices * sizeof(size_t))),
        *vals = (size_t *)(malloc(graph->vertices * sizeof(size_t)));
    for (i = 0; i < graph->vertices; i++) {
        new_order[i] = i;
        vals[i] = ((struct dfs_t*)graph->v_list[i].d)->found_time;
        if (vals[i] == 0)
            vals[i] = INF;
    }

    for (i = 0; i < graph->vertices - 1; i++) {
        for (j = 0; j < graph->vertices - 1; j++) {
            if (vals[j] < vals[j + 1]) {
                swap(vals[j], vals[j + 1]);
                swap(new_order[j], new_order[j + 1]);
            }
        }
    }
    free(vals);
    return new_order;
}

void find_scc(char* file)
{
    FILE *textfile;
    int scc = 0;
    textfile = fopen(file, "r");

    if (textfile == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }

    struct graph_t *g = malloc_graph(textfile); 
    fclose(textfile);
    for (size_t i = 0; i < g->vertices; i++)
        dfs(g, &g->v_list[i], 0);

    struct graph_t *t = graph_transpose(g);

    size_t *order = bubble_sort(g);

    for (size_t i = 0; i < t->vertices; i++)
        dfs(t, &t->v_list[order[i]], t->vertices < 100 ? 1 : 0);
    printf("Grafen fra fil '%s' hadde %d komponenter.\n", file, scc);
    // printf("\n");
    free(order);
    free(t);
    free(g);
}


int main()
{
    char* file_1 = "ø6g1.txt";
    char* file_2 = "ø6g2.txt";
    char* file_3 = "ø6g5.txt";
    char* file_4 = "ø6g6.txt";
    find_scc(file_1);
    find_scc(file_2);
    find_scc(file_3);
    find_scc(file_4);

    return 0;
}