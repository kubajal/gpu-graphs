
typedef enum {
    ATTR_UNSIGNED,
    ATTR_INT,
    ATTR_FLOAT,
    ATTR_PTR
} AttributeType;

typedef struct {
    AttributeType type;
    union {
        unsigned u;
        int i;
        float f;
        void *ptr;
    } value;
} Attribute;

typedef struct {
    int source;
    int target;
    Attribute data;
} Edge;

// CSR representation of a graph
typedef struct {
    int nodes_n;
    int edges_n;
    const Edge* edges;
    const Attribute* nodes;
} Graph;
