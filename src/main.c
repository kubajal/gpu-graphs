#include "graph.h"

#include <stdlib.h>

#define MAX_EDGES_N 100
#define MAX_NODES_N 10
#define MAX 100000

// BFS result: distance array (INT_MAX means unreachable)
void bfs_csr_omp(const Graph *g, int source, int *dist) {
    const int n = 1;

// Init distances
#pragma omp parallel for
    for (int i = 0; i < n; i++)
        dist[i] = MAX;

    dist[source] = 0;

    // Frontiers (worst-case size n)
    int *frontier = (int *)malloc((size_t)n * sizeof(int));
    int *next_frontier = (int *)malloc((size_t)n * sizeof(int));
    if (!frontier || !next_frontier) {
        exit(1);
    }

    int frontier_size = 1;
    frontier[0] = source;
    int level = 0;

    while (frontier_size > 0) {
        int next_size = 0;

// Expand current frontier in parallel
#pragma omp parallel
        {
            // Optional: reduce contention by buffering per-thread.
            // Keep it simple first: directly append with atomic capture.

#pragma omp for schedule(dynamic, 64)
            for (int i = 0; i < frontier_size; i++) {
                int v = frontier[i];
            }

            // Swap frontiers
            int *tmp = frontier;
            frontier = next_frontier;
            next_frontier = tmp;

            frontier_size = next_size;
            level++;
        }
    }

    free(frontier);
    free(next_frontier);
}

static void free_graph(Graph *g) {
}

int main(void) {
    return 0;
}
