#include "polyhit.h"

int polyhit(edge_t edges[], point_t hitpoint, int numsides) {
    int ndx;
    tuple_t hitPointRay;
    tuple_t edgeCrossHit;
    tuple_t oldEdgeCrossHit;
    for (ndx = 0; ndx < numsides; ndx++) {
        hitPointRay = ray(edges[ndx].point, hitpoint);
        hitPointRay = unitize(hitPointRay);
        edgeCrossHit = cross(edges[ndx].side,hitPointRay);
        if (length(edgeCrossHit) == 0) {
            return (0);
        }
        edgeCrossHit = unitize(edgeCrossHit);
        if (ndx != 0) {
            if(dot(edgeCrossHit,oldEdgeCrossHit) <= 0 ) {
                return(0);
            }
        }
        oldEdgeCrossHit = edgeCrossHit;
    }
    return (1);
}
