/* plane.c */

#include "ray.h"

int hitPlane(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    double distance;
    point_t hitpoint;

    assert(ent->magic == ENTITY_T);

    sobj_t *sptr = ent->entDerived;
    assert(sptr->magic == SCENEOBJ_T);
    
    plane_t *planePtr = sptr->sobjDerived;
    assert(planePtr->magic == PLANE_T);

    tuple_t N = planePtr->normal;
    tuple_t Q = planePtr->point;
    tuple_t D = dir;
    tuple_t V = base;
    
    if (dot(N,D) == 0) {
        return 0;
    }
    
    distance = (dot(N,Q)-dot(N,V))/(dot(N,D));

    if (distance < 0) {
        return 0;
    }
    
    hitpoint = add(V,scale(D,distance));

    if (hitpoint.z > 0) {
        return 0;
    }

    hit->hitpoint = hitpoint;
    hit->distance = distance;
    hit->normal = N;

    return(1);
}

