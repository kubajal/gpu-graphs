#!/usr/bin/env python3


from pathlib import Path
import sys


module_dir = Path("/workspace/bindings/python/src")
sys.path.append(str(module_dir))

import gpu_graphlib as gpu

static_logger = gpu.get_static_logger()
gpu.set_log_level(static_logger, gpu.LOG_LEVEL_DEBUG)

ATTR_UNSIGNED = 0

[edges, nodes, attributes_lookup] = gpu.ast_to_tree("/workspace/data/sample0.py")

graph = gpu.create_graph(
    {
        0: (ATTR_UNSIGNED, 0),
        1: (ATTR_UNSIGNED, 1),
        2: (ATTR_UNSIGNED, 2)
    },
    {
        0: [1],
        1: [2]
    })

SOURCE = 0
TARGET = 2
distance = gpu.dijkstra(graph, SOURCE, TARGET, static_logger)
print(f"Distance {SOURCE}->{TARGET} = {distance}")