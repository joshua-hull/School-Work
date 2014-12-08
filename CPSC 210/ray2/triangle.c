#include "ray.h"

entity_t *newTriangle(char *enttype, int code) {
    entity_t *returnEntity = newPlane(enttype, code);
    assert(returnEntity->magic == ENTITY_T);

    sobj_t *planeObj = returnEntity->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;    

    triangle_t *triangle = malloc(sizeof(triangle_t));
    triangle->magic = TRIANGLE_T;

    plane->planeDerived = triangle;
    
    
    returnEntity->load = loadTriangle;
    returnEntity->complete = completeTriangle;
    returnEntity->dump = dumpTriangle;

    triangle->length1 = 1;
    triangle->length2 = 1;
    
    ((sobj_t *)(returnEntity->entDerived))->hit = hitTriangle;

    return(returnEntity);
}
void loadTriangle(FILE *inFP, entity_t *ent, char *attribute) {
    char *attributes[]={"length1","length2",NULL};
    int ndx;

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);
    
    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    triangle_t *triangle = plane->planeDerived;
    assert(triangle->magic == TRIANGLE_T);

    ndx = getindex(attribute, attributes);
    
    switch(ndx) {
        case 0:
            triangle->length1 = readDouble(inFP, "Could not read lenght1.");
            break;
        case 1:
            triangle->length2 = readDouble(inFP, "Could not read length2.");
            break;
        default:
            loadPlane(inFP,ent,attribute);
            break;
    }
}

void completeTriangle(scene_t *scene, entity_t *ent) {

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    triangle_t *triangle = plane->planeDerived;
    assert(triangle->magic == TRIANGLE_T);

    edge_t *edges = triangle->edges;

    completePlane(scene,ent);    

    edges[0].point = plane->point;
    edges[0].side = unitize(plane->orient1);

    edges[1].point = add(edges[0].point,scale(unitize(plane->orient1),triangle->length1));
    edges[2].point = add(edges[0].point,scale(unitize(plane->orient2),triangle->length2));

    edges[2].side = unitize(scale(plane->orient2,-1.0));
    edges[1].side = unitize(ray(edges[1].point,edges[2].point));
    
}

void dumpTriangle(FILE *outFP, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);    

    dumpPlane(outFP, ent);
    
    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    triangle_t *triangle = plane->planeDerived;
    assert(triangle->magic == TRIANGLE_T);

    fprintf(outFP,"   length1:     %7.4lf\n",triangle->length1);
    fprintf(outFP,"   length2:     %7.4lf\n",triangle->length2);
    printEdge(outFP,triangle->edges,3);
}

int hitTriangle(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    
    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    triangle_t *triangle = plane->planeDerived;
    assert(triangle->magic == TRIANGLE_T);

    if (hitPlane(ent, base, dir, hit) == 1) { 
        return(polyhit(triangle->edges,hit->hitpoint, 3));
    } else {
        return(0);
    }   
}
