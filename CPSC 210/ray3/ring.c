#include "ray.h"

entity_t *newRing(char *enttype, int code) {
    entity_t *returnEntity = newPlane(enttype, code);
    assert(returnEntity->magic == ENTITY_T);

    sobj_t *planeObj = returnEntity->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;    

    ring_t *ring = malloc(sizeof(ring_t));
    ring->magic = RING_T;

    plane->planeDerived = ring;
    
    
    returnEntity->load = loadRing;
    returnEntity->complete = completeRing;
    returnEntity->dump = dumpRing;

    ring->radius1 = 1;
    ring->radius2 = 2;
    
    ((sobj_t *)(returnEntity->entDerived))->hit = hitRing;

    return(returnEntity);
}
void loadRing(FILE *inFP, entity_t *ent, char *attribute) {
    char *attributes[]={"radius1","radius2",NULL};
    int ndx;

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);
    
    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    ring_t *ring = plane->planeDerived;
    assert(ring->magic == RING_T);

    ndx = getindex(attribute, attributes);
    
    switch(ndx) {
        case 0:
            ring->radius1= readDouble(inFP, "Could not read radius1.");
            break;
        case 1:
            ring->radius2 = readDouble(inFP, "Could not read radius2.");
            break;
        default:
            loadPlane(inFP,ent,attribute);
            break;
    }
}
void completeRing(scene_t *scene, entity_t *ent) {

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    ring_t *ring = plane->planeDerived;
    assert(ring->magic == RING_T);

    completePlane(scene,ent);        
}
void dumpRing(FILE *outFP, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);    

    dumpPlane(outFP, ent);
    
    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    ring_t *ring = plane->planeDerived;
    assert(ring->magic == RING_T);

    fprintf(outFP,"   radius1:     %7.4lf\n",ring->radius1);
    fprintf(outFP,"   radius2:     %7.4lf\n",ring->radius2);
}
int hitRing(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    
    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    ring_t *ring= plane->planeDerived;
    assert(ring->magic == RING_T);

    if (hitPlane(ent, base, dir, hit) == 1) { 
        vector_t radiusRay = ray(plane->point,hit->hitpoint);
        double radiusLength = length(radiusRay);
        if(radiusLength >= ring->radius1 && radiusLength <= ring->radius2) {
            return(1);
        } else {
            return(0);
        }       
    } else {
        return(0);
    }   
}
