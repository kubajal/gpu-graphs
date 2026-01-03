
#include "graph.h"
#include "logger.h"

#include <omp.h>
#include <string.h>

#define MAX_DISTANCE 100000
#define MAX_NODES 100000
uint32_t dijkstra(const Graph *g, int32_t source, int32_t target, const Logger *log) {
    CSRGraph *graph = to_csr(g);
    uint32_t *distances = malloc(graph->nodes_n * sizeof(uint32_t));

    // Two frontiers for level-by-level processing
    node_id_t *curr_frontier = malloc(graph->nodes_n * sizeof(node_id_t));
    node_id_t *next_frontier = malloc(graph->nodes_n * sizeof(node_id_t));

    #pragma omp parallel for
    for (unsigned i = 0; i < graph->nodes_n; i++) distances[i] = MAX_DISTANCE;

    distances[source] = 0;
    curr_frontier[0] = source;
    uint32_t curr_size = 1;
    char found = 0;

    while (curr_size > 0 && !found) {
        uint32_t next_size = 0;

        // Parallelize the processing of the current "level"
        #pragma omp parallel
        {
            // Local buffer to avoid contention on the global next_frontier
            node_id_t *local_next = malloc(graph->nodes_n * sizeof(node_id_t));
            uint32_t local_count = 0;

            #pragma omp for nowait
            for (uint32_t i = 0; i < curr_size; i++) {
                node_id_t parent = curr_frontier[i];

                for (uint32_t j = graph->col_ptr[parent]; j < graph->col_ptr[parent+1]; j++) {
                    node_id_t child = graph->node_ptr[j];

                    // In a tree, we only visit unvisited nodes (distance == MAX)
                    // Use Atomic to prevent race conditions if this weren't a strict tree
                    if (distances[child] == MAX_DISTANCE) {
                        distances[child] = distances[parent] + 1;
                        local_next[local_count++] = child;
                        if (child == target) found = 1;
                    }
                }
            }

            // Merge local results into the global next_frontier
            uint32_t offset;
            #pragma omp atomic capture
            { offset = next_size; next_size += local_count; }

            memcpy(&next_frontier[offset], local_next, local_count * sizeof(node_id_t));
            free(local_next);
        }

        // Swap buffers for the next level
        node_id_t *temp = curr_frontier;
        curr_frontier = next_frontier;
        next_frontier = temp;
        curr_size = next_size;
    }

    uint32_t result = distances[target];
    LOG_DEBUG(log, "result: %u", result);
    free(distances);
    free(graph->node_ptr);
    free(graph->col_ptr);
    return result;
}

