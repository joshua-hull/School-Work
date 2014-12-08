typedef struct edge {
    point_t point;
    vector_t side;
} edge_t;

void printEdge(FILE *outFP, edge_t *edges, int numEdges);
