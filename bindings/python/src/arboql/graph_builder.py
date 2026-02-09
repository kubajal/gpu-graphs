
from .ctype_bindings import add_edge, add_node, malloc_graph
ATTR_UNSIGNED = 0

def create_graph(nodes, edges):
    nodes_n = len(nodes)
    edges_n = 0
    for source in edges:
        for target in edges[source]:
            edges_n = edges_n + 1
    graph = malloc_graph(nodes_n, edges_n)
    for node, (attribute_type, attribute_value) in nodes.items():
        add_node(graph, node, attribute_type, attribute_value)
    edges_counter = 0
    for source in edges:
        for target in edges[source]:
            add_edge(
                graph,
                edges_counter,
                source,
                target,
                ATTR_UNSIGNED,
                target,
            )
            edges_counter = edges_counter + 1

    return graph
