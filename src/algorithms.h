
#include <stdio.h>
#include <stdint.h>

#include "graph.h"
#include "logger.h"

typedef struct {
    size_t n, k;

    // current partial assignment a[0..pos-1]
    size_t *a;          // length k
    size_t *nextc;      // next candidate to try at each position, length k
    unsigned char *used;// used[v] = 1 if v in current prefix, length n

    size_t pos;         // current depth (0..k)
    int started;
    int done;

    int owns_mem;
} inj_coroutine;
static int inj_init(inj_coroutine *g,
                    size_t n, size_t k,
                    size_t *a_storage,        // optional (NULL => allocate)
                    size_t *nextc_storage,    // optional (NULL => allocate)
                    unsigned char *used_storage); // optional (NULL => allocate)
static void inj_destroy(inj_coroutine *g);
static int inj_next(inj_coroutine *g, int *out_images /* length k */);


typedef struct {
    const int **sets;     // sets[i] points to array of ints
    const size_t *sizes;  // sizes[i] is length of sets[i]
    size_t num_sets;

    size_t *idx;          // mixed-radix counter indices[num_sets]
    int started;          // 0 before first yield, 1 after
    int done;             // 1 when finished
} cp_coroutine;
static int cp_init(cp_coroutine *cp,
                   const int **sets,
                   const size_t *sizes,
                   size_t num_sets,
                   size_t *idx_storage /* optional: pass NULL to allocate */);
static void cp_destroy(cp_coroutine *cp, size_t *idx_storage /* same pointer you passed to init */);
static int cp_next(cp_coroutine *cp, int *out_values /* length num_sets */);

uint32_t match(const Graph *ast, const Graph *pattern, const Logger *log);
