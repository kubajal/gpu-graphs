
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms.h"

/*
  Cartesian product coroutine (stackless):
  - Call cp_init()
  - Repeatedly call cp_next(); it returns 1 if it yielded a tuple, 0 when done.
  - The yielded tuple is written into out_values[num_sets].
*/

static int cp_init(cp_coroutine *cp,
                   const int **sets,
                   const size_t *sizes,
                   size_t num_sets,
                   size_t *idx_storage /* optional: pass NULL to allocate */)
{
    if (!cp || !sets || !sizes || num_sets == 0) return 0;

    // If any set is empty, product is empty.
    for (size_t i = 0; i < num_sets; i++) {
        if (sizes[i] == 0) {
            cp->done = 1;
            cp->started = 0;
            cp->sets = sets;
            cp->sizes = sizes;
            cp->num_sets = num_sets;
            cp->idx = idx_storage; // may be NULL; irrelevant when done=1
            return 1;
        }
    }

    cp->sets = sets;
    cp->sizes = sizes;
    cp->num_sets = num_sets;
    cp->started = 0;
    cp->done = 0;

    if (idx_storage) {
        cp->idx = idx_storage;
    } else {
        cp->idx = (size_t*)calloc(num_sets, sizeof(size_t));
        if (!cp->idx) return 0;
    }

    // Initialize indices to 0,0,0...
    memset(cp->idx, 0, num_sets * sizeof(size_t));
    return 1;
}

static void cp_destroy(cp_coroutine *cp, size_t *idx_storage /* same pointer you passed to init */)
{
    if (!cp) return;
    // Free only if we allocated internally
    if (cp->idx && cp->idx != idx_storage) free(cp->idx);
    cp->idx = NULL;
}

static int cp_next(cp_coroutine *cp, int *out_values /* length num_sets */)
{
    if (!cp || !out_values) return 0;
    if (cp->done) return 0;

    // First resume yields the initial tuple (all indices = 0)
    if (!cp->started) {
        cp->started = 1;
    } else {
        // Increment mixed-radix counter (advance to next tuple)
        for (size_t k = cp->num_sets; k-- > 0;) {
            cp->idx[k]++;
            if (cp->idx[k] < cp->sizes[k]) {
                break; // carry resolved
            }
            cp->idx[k] = 0; // carry
            if (k == 0) {
                cp->done = 1; // overflowed most significant digit
                return 0;
            }
        }
    }

    // Materialize current tuple
    for (size_t i = 0; i < cp->num_sets; i++) {
        out_values[i] = cp->sets[i][cp->idx[i]];
    }

    return 1; // yielded
}
