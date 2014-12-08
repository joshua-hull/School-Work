#include "ray.h"

entity_t *newRectangle(char *enttype, int code) {
    entity_t *returnEntity = newPlane(enttype, code);
    assert(returnEntity->magic == ENTITY_T);
    
    sobj_t *planeobj = returnEntity->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rectangle = malloc(sizeof(rectangle_t));
    plane->planeDerived = rectangle;
    
    rectangle->width = 1;
    rectangle->height = 1;

    rectangle->magic = RECTANGLE_T;
    returnEntity->load = loadRectangle;
    returnEntity->complete = completeRectangle;
    returnEntity->dump = dumpRectangle;

    ((sobj_t *)(returnEntity->entDerived))->hit = hitRectangle;

    return(returnEntity);
}

void loadRectangle(FILE *inFP, entity_t *ent, char *attribute) {
    
    char *attributes[] = {"height", "width", NULL};
    int ndx;

    assert(ent->magic == ENTITY_T);

    sobj_t *planeObj = ent->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;
    assert(plane->magic = PLANE_T);

    rectangle_t *rectangle = plane->planeDerived;
    assert(rectangle->magic == RECTANGLE_T);

    ndx = getindex(attribute,attributes);
    
    switch(ndx) {
        case 0:
            rectangle->height = readDouble(inFP, "Could not read height.");
            break;
        case 1:
            rectangle->width = readDouble(inFP, "Could not read width.");
            break;
        default:
            loadPlane(inFP, ent, attribute);
            break;
    }
}

void completeRectangle(scene_t *scene, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);
    
    sobj_t *planeObj = ent->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rectangle = plane->planeDerived;
    assert(rectangle->magic == RECTANGLE_T);

    edge_t *edge = rectangle->edges;

    completePlane(scene, ent);

    edge[0].point = plane->point;
    edge[0].side = unitize(plane->orient1);
    
    edge[1].point = add(edge[0].point,scale(unitize(plane->orient1),
        rectangle->width));
    edge[1].side = unitize(cross(plane->normal, plane->orient1));

    edge[2].point = add(edge[1].point, scale(edge[1].side, rectangle->height));
    edge[2].side = scale(edge[0].side, -1.0);

    edge[3].point = add(edge[2].point, scale(edge[2].side, rectangle->width));
    edge[3].side = scale(edge[1].side,-1.0);
}

void dumpRectangle(FILE *outFP, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);
    
    sobj_t *planeObj = ent->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rectangle = plane->planeDerived;
    assert(rectangle->magic == RECTANGLE_T);

    dumpPlane(outFP, ent);

    fprintf(outFP, "    width:    %7.4lf\n",rectangle->width);
    fprintf(outFP, "    height:   %7.4lf\n",rectangle->height);

    printEdge(outFP,rectangle->edges,4);
}

int hitRectangle(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    
    assert(ent->magic == ENTITY_T);

    sobj_t *planeObj = ent->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);
    
    plane_t *plane  = planeObj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rectangle = plane->planeDerived;
    assert(rectangle->magic == RECTANGLE_T);

    if(hitPlane(ent, base, dir, hit) == 1) {
        return(polyhit(rectangle->edges, hit->hitpoint,4));
    } else {
        return(0);
    }
}
