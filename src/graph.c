
#include "graph.h"
#include "logger.h"
#include <stdlib.h>

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
