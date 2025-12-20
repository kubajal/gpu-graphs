// main.c
// Minimal OpenMP "tree" program that runs unchanged on CPU or GPU.
// It does a bottom-up DP on a tree (CSR) using level-by-level processing.
//
// Build CPU:   clang -O2 -fopenmp main.c -o tree
// (GPU build needs OpenMP offload flags; see notes below)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define CHECK(cond, msg)                                  \
  do {                                                    \
    if (!(cond)) {                                        \
      fprintf(stderr, "ERROR: %s\n", msg);                \
      exit(1);                                            \
    }                                                     \
  } while (0)

// CSR tree
typedef struct {
  int32_t n;
  int64_t m;
  int64_t *off;   // n+1
  int32_t *ind;   // m
} CSR;

static void csr_free(CSR *t) {
  if (!t) return;
  free(t->off);
  free(t->ind);
  memset(t, 0, sizeof(*t));
}

// Tiny fixed tree:
// 0 -> {1,2}
// 1 -> {3}
// 2 -> {}
// 3 -> {}
static CSR make_tiny_tree(void) {
  CSR t = {0};
  t.n = 4;
  t.m = 3;

  t.off = (int64_t*)malloc((size_t)(t.n + 1) * sizeof(int64_t));
  t.ind = (int32_t*)malloc((size_t)t.m * sizeof(int32_t));
  CHECK(t.off && t.ind, "malloc failed");

  // adjacency packed:
  // children(0) = [1,2]
  // children(1) = [3]
  // children(2) = []
  // children(3) = []
  t.off[0] = 0; t.off[1] = 2; t.off[2] = 3; t.off[3] = 3; t.off[4] = 3;
  t.ind[0] = 1; t.ind[1] = 2; t.ind[2] = 3;

  return t;
}

// Precompute node depths (levels) on host for a tree.
// For this tiny demo we just hardcode them to keep it minimal.
// level[0]=0, level[1]=1, level[2]=1, level[3]=2
static int32_t compute_levels_tiny(const CSR *t, int32_t *level_out) {
  (void)t;
  level_out[0] = 0;
  level_out[1] = 1;
  level_out[2] = 1;
  level_out[3] = 2;
  return 2; // max_level
}

// Create nodes_by_level[] and level_offsets[] such that:
// nodes_by_level[level_offsets[L] .. level_offsets[L+1]) are nodes at level L
static void group_by_level(const int32_t *level, int32_t n, int32_t max_level,
                           int32_t *level_offsets /* size max_level+2 */,
                           int32_t *nodes_by_level /* size n */) {
  // count per level
  for (int32_t L = 0; L <= max_level + 1; ++L) level_offsets[L] = 0;
  for (int32_t u = 0; u < n; ++u) {
    CHECK(level[u] >= 0 && level[u] <= max_level, "bad level");
    level_offsets[level[u] + 1]++; // shift by 1 for prefix sum
  }

  // prefix sum -> offsets
  for (int32_t L = 0; L < max_level + 1; ++L)
    level_offsets[L + 1] += level_offsets[L];

  // fill
  int32_t *cursor = (int32_t*)malloc((size_t)(max_level + 1) * sizeof(int32_t));
  CHECK(cursor, "malloc failed");
  for (int32_t L = 0; L <= max_level; ++L) cursor[L] = level_offsets[L];

  for (int32_t u = 0; u < n; ++u) {
    int32_t L = level[u];
    nodes_by_level[cursor[L]++] = u;
  }

  free(cursor);

  // After this:
  // level_offsets[0] = 0
  // level_offsets[max_level+1] = n
  // and each level slice is contiguous.
}

int main(void) {
#ifdef _OPENMP
  printf("OpenMP enabled. max_threads=%d\n", omp_get_max_threads());
#else
  printf("OpenMP not enabled (compile with OpenMP flags).\n");
#endif

  CSR t = make_tiny_tree();

  int32_t *value = (int32_t*)malloc((size_t)t.n * sizeof(int32_t));
  uint8_t *contains = (uint8_t*)calloc((size_t)t.n, sizeof(uint8_t));
  int32_t *level = (int32_t*)malloc((size_t)t.n * sizeof(int32_t));
  CHECK(value && contains && level, "alloc failed");

  value[0] = 10;
  value[1] = 20;
  value[2] = 30;
  value[3] = 42;

  const int32_t target = 42;

  int32_t max_level = compute_levels_tiny(&t, level);

  int32_t *nodes_by_level = (int32_t*)malloc((size_t)t.n * sizeof(int32_t));
  int32_t *level_offsets = (int32_t*)malloc((size_t)(max_level + 2) * sizeof(int32_t));
  CHECK(nodes_by_level && level_offsets, "alloc failed");

  group_by_level(level, t.n, max_level, level_offsets, nodes_by_level);

  // --------- Offload-friendly DP (works on CPU too) ---------
  //
  // Base: contains[u] = (value[u] == target)
  // For L from max_level-1 down to 0:
  //   for u in level L in parallel:
  //     contains[u] |= OR_{child v} contains[v]
  //
  // This structure maps well to GPU (kernel per level) and also runs on CPU
  // when no GPU offload is available.

#pragma omp target data \
  map(to: t.off[0:t.n+1], t.ind[0:t.m], value[0:t.n], nodes_by_level[0:t.n], level_offsets[0:max_level+2]) \
  map(tofrom: contains[0:t.n])
  {
    // init
#pragma omp target teams distribute parallel for
    for (int32_t u = 0; u < t.n; ++u) {
      contains[u] = (value[u] == target) ? 1u : 0u;
    }

    // bottom-up by levels
    for (int32_t L = max_level - 1; L >= 0; --L) {
      int32_t begin = level_offsets[L];
      int32_t end   = level_offsets[L + 1];

#pragma omp target teams distribute parallel for
      for (int32_t i = begin; i < end; ++i) {
        int32_t u = nodes_by_level[i];
        uint8_t hit = contains[u];
        for (int64_t e = t.off[u]; e < t.off[u + 1] && !hit; ++e) {
          int32_t v = t.ind[e];
          hit |= contains[v];
        }
        contains[u] = hit;
      }
      // Each target region launch acts like a global sync between levels.
    }
  }

  // Print results
  printf("Target %d exists anywhere? %s\n", target, contains[0] ? "YES" : "NO");
  for (int32_t u = 0; u < t.n; ++u) {
    printf("subtree(%d) contains %d? %s (value=%d)\n",
           u, target, contains[u] ? "YES" : "NO", value[u]);
  }

  free(level_offsets);
  free(nodes_by_level);
  free(level);
  free(contains);
  free(value);
  csr_free(&t);
  return 0;
}
