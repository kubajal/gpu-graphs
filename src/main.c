#include "algorithms.h"
#include "graph.h"
#include "tgf.h"
#include <unistd.h>

#define MAX_EDGES_N 100
#define MAX_NODES_N 10

int main(int argc, char *argv[]) {
    const Graph graph = parse(argv[1]);
    bfs(&graph, 0, 1);
    return 0;
}
