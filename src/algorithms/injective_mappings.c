
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms.h"

/*
  Enumerate all injections f: [0..k-1] -> [0..n-1] (distinct images).

  Usage:
    inj_coroutine gen;
    size_t a[k], nextc[k];
    unsigned char used[n];
    inj_init(&gen, n, k, a, nextc, used); // no heap
    while (inj_next(&gen, out int[k])) { ... }
*/

static int inj_init(inj_coroutine *g,
                    size_t n, size_t k,
                    size_t *a_storage,        // optional (NULL => allocate)
                    size_t *nextc_storage,    // optional (NULL => allocate)
                    unsigned char *used_storage) // optional (NULL => allocate)
{
    if (!g) return 0;
    g->n = n;
    g->k = k;
    g->pos = 0;
    g->started = 0;
    g->done = 0;
    g->owns_mem = 0;

    // Empty domain: exactly one mapping (empty tuple)
    if (k == 0) return 1;

    // If k > n, there are no injections.
    if (k > n) {
        g->done = 1;
        return 1;
    }

    if (a_storage && nextc_storage && used_storage) {
        g->a = a_storage;
        g->nextc = nextc_storage;
        g->used = used_storage;
    } else {
        g->a = (size_t*)malloc(k * sizeof(size_t));
        g->nextc = (size_t*)malloc(k * sizeof(size_t));
        g->used = (unsigned char*)malloc(n * sizeof(unsigned char));
        if (!g->a || !g->nextc || !g->used) {
            free(g->a); free(g->nextc); free(g->used);
            return 0;
        }
        g->owns_mem = 1;
    }

    memset(g->used, 0, n * sizeof(unsigned char));
    for (size_t i = 0; i < k; i++) {
        g->a[i] = 0;
        g->nextc[i] = 0;
    }
    return 1;
}

static void inj_destroy(inj_coroutine *g)
{
    if (!g) return;
    if (g->owns_mem) {
        free(g->a);
        free(g->nextc);
        free(g->used);
    }
    g->a = NULL;
    g->nextc = NULL;
    g->used = NULL;
}

/*
  Yield next injection.
  out_images[i] = f(i) in [0..n-1]
  Returns 1 if a mapping is produced, 0 if finished.
*/
static int inj_next(inj_coroutine *g, int *out_images /* length k */)
{
    if (!g || (g->k && !out_images)) return 0;
    if (g->done) return 0;

    // Special-case: k == 0 => yield once.
    if (g->k == 0) {
        if (g->started) return 0;
        g->started = 1;
        return 1;
    }

    // Depth-first search with explicit state; each complete assignment is a yield.
    for (;;) {
        if (g->pos == g->k) {
            // Found a complete injection; materialize output.
            for (size_t i = 0; i < g->k; i++) {
                out_images[i] = (int)g->a[i];
            }

            // Prepare state to resume: backtrack one level and continue from next candidate.
            g->pos = g->k - 1;
            g->used[g->a[g->pos]] = 0;
            // nextc[pos] already points to (last_chosen + 1) because of the assignment step below.
            return 1;
        }

        // Try next available value at current position.
        size_t v = g->nextc[g->pos];
        while (v < g->n && g->used[v]) v++;

        if (v < g->n) {
            // Choose v at this position.
            g->a[g->pos] = v;
            g->used[v] = 1;
            g->nextc[g->pos] = v + 1; // next time at this pos, start after v
            g->pos++;
            if (g->pos < g->k) g->nextc[g->pos] = 0; // new position: start from 0
            continue;
        }

        // No candidates left at this position: reset and backtrack.
        g->nextc[g->pos] = 0;
        if (g->pos == 0) {
            g->done = 1;
            return 0;
        }
        g->pos--;
        g->used[g->a[g->pos]] = 0;
        // continue loop; we'll try the next candidate at the backtracked position
    }
}
