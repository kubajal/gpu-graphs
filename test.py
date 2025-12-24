import gpu_graphlib as gpu

static_logger = gpu.get_static_logger()
gpu.set_log_level(static_logger, gpu.LOG_LEVEL_DEBUG)

ATTR_UNSIGNED = 0

[edges, nodes, attributes_lookup] = gpu.ast_to_tree("./data/sample0.py")

graph = gpu.create_graph(nodes, edges)
gpu.bfs(graph, 0, 0, static_logger)
