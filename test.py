#!/usr/bin/env python3


from pathlib import Path
import sys

import arboql

static_logger = arboql.get_static_logger()
arboql.set_log_level(static_logger, arboql.LOG_LEVEL_INFO)

ATTR_UNSIGNED = 0

print("Creating graph")
graph = arboql.create_graph(
    {
        0: (ATTR_UNSIGNED, 0),
        1: (ATTR_UNSIGNED, 1),
        2: (ATTR_UNSIGNED, 2)
    },
    {
        0: [1],
        1: [2]
    })


pattern = arboql.create_graph(
    {
        0: (ATTR_UNSIGNED, 0),
        1: (ATTR_UNSIGNED, 1)
    },
    {
        0: [1]
    })

print("Running pattern matching")
status = arboql.match(graph, pattern, static_logger)
arboql.free_graph(graph)
print(f"Status {status}")
