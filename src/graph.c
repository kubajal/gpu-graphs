
#include "graph.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>

Graph *malloc_graph(unsigned nodes_n, unsigned edges_n) {
    Graph *graph = malloc(sizeof(Graph));
    graph->edges = malloc(sizeof(Edge) * edges_n);
    graph->nodes = malloc(sizeof(Node) * nodes_n);
    graph->edges_n = edges_n;
    graph->nodes_n = nodes_n;
    LOG_DEBUG(get_static_logger(), "Allocated new graph (%d nodes and %d edges)", nodes_n, edges_n);
    return graph;
}

void free_graph(Graph *graph) {
    unsigned edges_n = graph->edges_n;
    unsigned nodes_n = graph->nodes_n;
    free(graph->edges);
    free(graph->nodes);
    free(graph);
    LOG_DEBUG(get_static_logger(), "Freed graph (%d nodes and %d edges)", nodes_n, edges_n);
}

void add_edge(Graph *graph, const unsigned edge_id, const unsigned source, const unsigned target,
              const AttributeType attribute_type, const AttributeValue attribute_value) {
    graph->edges[edge_id].source = source;
    graph->edges[edge_id].target = target;
    graph->edges[edge_id].data.type = attribute_type;
    graph->edges[edge_id].data.value = attribute_value;
}

void add_node(Graph *graph, const unsigned node_id, const AttributeType attribute_type,
              const AttributeValue attribute_value) {
    graph->nodes[node_id].type = attribute_type;
    graph->nodes[node_id].value = attribute_value;
}

CSRGraph* to_csr(const Graph *graph) {
    if (graph == NULL) return NULL;

    CSRGraph *csr = (CSRGraph*)malloc(sizeof(CSRGraph));
    csr->nodes_n = graph->nodes_n;
    csr->edges_n = graph->edges_n;

    // Allocate the arrays
    // col_ptr size is nodes_n + 1 to store the end offset of the last node
    csr->col_ptr = (node_id_t*)calloc(graph->nodes_n + 1, sizeof(node_id_t));
    csr->node_ptr = (node_id_t*)malloc(graph->edges_n * sizeof(node_id_t));

    // 1. Count the degree of each node
    for (node_id_t i = 0; i < graph->edges_n; i++) {
        node_id_t src = graph->edges[i].source;
        csr->col_ptr[src + 1]++;
    }

    // 2. Compute Prefix Sum (Cumulative sum)
    // This turns counts into starting offsets
    for (node_id_t i = 0; i < graph->nodes_n; i++) {
        csr->col_ptr[i + 1] += csr->col_ptr[i];
    }

    // 3. Fill node_ptr
    // We need a temporary counter to keep track of where to insert the next edge
    node_id_t *current_pos = (node_id_t*)malloc((graph->nodes_n) * sizeof(node_id_t));
    memcpy(current_pos, csr->col_ptr, graph->nodes_n * sizeof(node_id_t));

    for (node_id_t i = 0; i < graph->edges_n; i++) {
        node_id_t src = graph->edges[i].source;
        node_id_t dest = graph->edges[i].target;

        node_id_t pos = current_pos[src];
        csr->node_ptr[pos] = dest;
        current_pos[src]++;
    }

    free(current_pos);
    return csr;
}