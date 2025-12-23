
#pragma once

typedef enum { ATTR_UNSIGNED, ATTR_INT, ATTR_FLOAT, ATTR_PTR } AttributeType;

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
  int source;
  int target;
  Attribute data;
} Edge;

typedef Attribute Node;

// CSR representation of a graph
typedef struct {
  int nodes_n;
  int edges_n;
  Edge *edges;
  Node *nodes;
} Graph;

Graph *malloc_graph(unsigned nodes_n, unsigned edges_n);
void free_graph(Graph *g);

Attribute *malloc_attribute(AttributeType type, AttributeValue value);
void free_attribute(Attribute *attribute);

void add_edge(Graph *graph, const unsigned edge_id, const unsigned source,
              const unsigned target, const AttributeType attribute_type,
              const AttributeValue attribute_value);
void add_node(Graph *graph, const unsigned node_id,
              const AttributeType attribute_type,
              const AttributeValue attribute_value);
