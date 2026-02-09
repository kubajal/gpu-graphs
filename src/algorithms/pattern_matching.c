
#include "graph.h"
#include "logger.h"

#include <string.h>
#include <stdint.h>

#define MAX_DISTANCE 100000
#define MAX_NODES 100000

uint32_t match(const Graph *ast, const Graph *pattern, const Logger *log) {
    print_graph(ast, 0, 0);
    print_graph(pattern, 0, 0);
    return 42;
}

