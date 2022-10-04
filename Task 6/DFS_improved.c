#include <stdio.h>      // fprintf, printf, fopen
#include <stdlib.h>
#include <string.h>     // memset

#define INF 1000000000

typedef struct edge_t edge_t;

typedef struct vertex_t vertex_t;

typedef struct stack_t stack_t;

typedef struct graph_t graph_t;

struct edge_t {
    struct vertex_t *to_vertex;
    struct edge_t *next_edge;
};

struct vertex_t {
    size_t node_index;
    struct edge_t *first_edge;
};

struct stack_t {
    size_t *data;
    size_t pos;
    size_t capacity;
};

struct graph_t {
    struct vertex_t *v_list;
    size_t vertices;
    size_t edges;
};

struct stack_t *malloc_stack(size_t capacity)
{
    struct stack_t *stack = malloc(sizeof(struct stack_t));
    stack->data = malloc(sizeof(size_t) * capacity);
    stack->pos = 0;
    stack->capacity = capacity;

    return stack;
}

void free_stack(struct stack_t *stack)
{
    free(stack->data);
    free(stack);
}

void stack_push(struct stack_t *stack, size_t value)
{
    if (stack->pos == stack->capacity) {
        fprintf(stderr, "stack full!");
        return;
    }

    stack->data[stack->pos++] = value;
}

size_t stack_pop(struct stack_t *stack)
{
    if (stack->pos == 0)
        return INF;

    return stack->data[--stack->pos];
}

int stack_is_empty(struct stack_t *stack)
{
    return stack->pos == 0;
}


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
        edge->next_edge = graph->v_list[from].first_edge;
        graph->v_list[from].first_edge = edge;
    }
    return graph;
}

struct graph_t *graph_transpose(struct graph_t *graph)
{
    size_t i, j;
    struct graph_t *t_graph = (struct graph_t *)(malloc(sizeof(struct graph_t)));
    t_graph->v_list = (struct vertex_t*)(calloc(graph->vertices, sizeof(struct vertex_t)));
    t_graph->vertices = graph->vertices;
    t_graph->edges = graph->edges;

    for (i = 0; i < t_graph->vertices; i++) {
        struct vertex_t *curr = &graph->v_list[i];
        t_graph->v_list[i].node_index = curr->node_index;
        for (struct edge_t *edge = curr->first_edge; edge; edge = edge->next_edge) {
            struct edge_t *new_edge = (struct edge_t*)(malloc(sizeof(struct edge_t)));
            new_edge->to_vertex = &t_graph->v_list[i];
            new_edge->next_edge = t_graph->v_list[edge->to_vertex->node_index].first_edge;
            t_graph->v_list[edge->to_vertex->node_index].first_edge = new_edge;
        }
    }
    return t_graph;
}

static void DFS(struct graph_t *graph, size_t start_node, int *visited)
{
    size_t node;
    struct stack_t *stack = malloc_stack(graph->edges);
    stack_push(stack, start_node);

    while (!stack_is_empty(stack)) {
        node = stack_pop(stack);
        if (!visited[node]) {
            visited[node] = 1;
            printf("%zu ", node);
            struct vertex_t *curr = &graph->v_list[node];
            stack_push(stack, curr->node_index);
            for (struct edge_t *edge = curr->first_edge; edge; edge = edge->next_edge)
                stack_push(stack, edge->to_vertex->node_index);
        }
    }
}

static void kosarajo_visit(struct graph_t *graph, size_t node, int *visited, struct stack_t *stack)
{
    visited[node] = 1;
    struct vertex_t *curr = &graph->v_list[node];
    if (!visited[curr->node_index])
        kosarajo_visit(graph, curr->node_index, visited, stack);
    for (struct edge_t *edge = curr->first_edge; edge; edge = edge->next_edge) {
        if (!visited[edge->to_vertex->node_index])
            kosarajo_visit(graph, edge->to_vertex->node_index, visited, stack);
    }

    stack_push(stack, node);
}

static void kosaraju(struct graph_t *graph)
{
    printf("Components\tNodes\n");
    struct stack_t *stack = malloc_stack(graph->edges);
    int visited[graph->vertices];
    memset(visited, 0, sizeof(visited));
    
    for (size_t i = 0; i < graph->vertices; i++)
        if (!visited[i]) 
            kosarajo_visit(graph, i, visited, stack);

    struct graph_t *transposed = graph_transpose(graph);

    memset(visited, 0, sizeof(visited));
    
    size_t component = 1;
    while (!stack_is_empty(stack)) {
        size_t node = stack_pop(stack);
        if (!visited[node]) {
            printf("%zu\t\t", component);
            DFS(transposed, node, visited);
            putchar('\n');
            component++;
        }
    }
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
    
    kosaraju(g);

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