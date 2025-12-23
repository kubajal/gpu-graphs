import gpu_graphlib as gpu

static_logger = gpu.get_static_logger()
gpu.set_log_level(static_logger, gpu.LOG_LEVEL_DEBUG)

def create_graph(nodes, edges):
    nodes_n = len(nodes)
    edges_n = 0
    for source in edges:
        for target in edges[source]:
            edges_n = edges_n + 1
    graph = gpu.malloc_graph(nodes_n, edges_n)
    for node, (attribute_type, attribute_value) in nodes.items():
        gpu.add_node(graph, node, attribute_type, attribute_value)
    edges_counter = 0
    for source in edges:
        for target in edges[source]:
            (attribute_type, attribute_value) = edges[source][target]
            gpu.add_edge(
                graph,
                edges_counter,
                source,
                target,
                attribute_type,
                attribute_value,
            )
            edges_counter = edges_counter + 1

    return graph

ATTR_UNSIGNED = 0

graph_builder = create_graph(
    {
        0: (ATTR_UNSIGNED, 0),
        1: (ATTR_UNSIGNED, 1),
        2: (ATTR_UNSIGNED, 2),
        3: (ATTR_UNSIGNED, 3)
    },
    {
        0: {
            1: (ATTR_UNSIGNED, 10),
            2: (ATTR_UNSIGNED, 11)
        },
        2: {
            3: (ATTR_UNSIGNED, 12)
        }
    })
