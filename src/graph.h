
#pragma once

#include <stdio.h>

typedef enum { ATTR_UNSIGNED, ATTR_INT, ATTR_FLOAT, ATTR_PTR } AttributeType;
typedef unsigned node_id_t;

typedef union {
  unsigned u;
  //   int i;
  //   float f;
  //   void *ptr;
} AttributeValue;

typedef struct {
  AttributeType type;
  AttributeValue value;
} Attribute;

typedef struct {
  node_id_t source;
  node_id_t target;
  Attribute data;
} Edge;

typedef Attribute Node;

typedef struct {
  node_id_t nodes_n;
  node_id_t edges_n;
  Edge *edges;
  Node *nodes;
} Graph;

typedef struct {
  node_id_t nodes_n;
  node_id_t edges_n;
  node_id_t* col_ptr;
  node_id_t* node_ptr;
} CSRGraph;

Graph *malloc_graph(node_id_t nodes_n, node_id_t edges_n);
void free_graph(Graph *g);

Attribute *malloc_attribute(AttributeType type, AttributeValue value);
void free_attribute(Attribute *attribute);

void add_edge(Graph *graph, const node_id_t edge_id, const node_id_t source,
              const node_id_t target, const AttributeType attribute_type,
              const AttributeValue attribute_value);
void add_node(Graph *graph, const node_id_t node_id,
              const AttributeType attribute_type,
              const AttributeValue attribute_value);
CSRGraph* to_csr(const Graph *graph);

void print_graph(const Graph *graph, node_id_t current, unsigned indent);