#include "ray.h"

void printEdge(FILE *outFP, edge_t *edges, int numEdges) {
    int i;
    for(i = 0; i < numEdges; i++) {
        fprintf(outFP, "   Edge %d:\n",i);
        fprintf(outFP, "      point:        ");
        printTuple(outFP, "",edges[i].point);
        fprintf(outFP, "      edge:         ");
        printTuple(outFP,"",edges[i].side);
    }
}
