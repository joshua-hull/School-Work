/* plane.c */

#include "ray.h"

/** newPlane **/
entity_t *newPlane(char *enttype, int code) {
    
    // Create a new entity to return.
    entity_t *returnEntity = newSceneObj(enttype,code);
    
    // Make some shrotcuts and do some sanity checks 
    sobj_t *obj = returnEntity->entDerived;
    assert(obj->magic == SCENEOBJ_T);

    // Create the plane specific parts, assign them, and return the entity.
    obj->sobjDerived = (plane_t *) malloc(sizeof(plane_t));
    plane_t *plane = obj->sobjDerived;
    plane->magic = PLANE_T;
    plane->point = (tuple_t){0,0,0};
    plane->normal = (tuple_t){0,0,1};
    plane->orient1 = (tuple_t){1,0,0};
    plane->orient2 = (tuple_t){0,1,0};
    return(returnEntity);
}

/** loadPlane **/
void loadPlane(FILE *inFP, entity_t *ent, char *attribute) {
    
    // List of attributes unique to a plane
    char *attributes[] = {"point", "orient1","orient2", NULL};
    int ndx;

    // Create some short cuts and do some sanity checks.
    assert(ent->magic == ENTITY_T);
    sobj_t *obj = ent->entDerived;
    assert(obj->magic == SCENEOBJ_T);
    plane_t *plane = obj->sobjDerived;
    assert(plane->magic == PLANE_T);
    
    // Find out which attribute we are dealing with
    ndx = getindex(attribute, attributes);
    
    switch (ndx) {
        case 0:
            // Point
            plane->point = readTuple(inFP,"Could not read plane point coordinates");
            break;
            
        case 1:
            // Orient1
            plane->orient1 = readTuple(inFP, "Could not read plane orient1 tuple");
            break;
            
        case 2:
            //Orient2
            plane->orient2 = readTuple(inFP, "Could not read plane orient2 tuple");
            break;
            
        default:
            // Not our responsibility so pass it up the chain.
            loadSceneObj(inFP, ent, attribute);
    }
}

/** completePlane **/
void completePlane(scene_t *scene, entity_t *ent) {
    
    // Do some shortcuts and sanity checks.
    sobj_t *obj = ent->entDerived;
    assert(obj->magic == SCENEOBJ_T);
    plane_t *plane = obj->sobjDerived;
    assert(plane->magic == PLANE_T);
    
    // Calculate the normal, unitize it, and add it to the plane_t structure.
    tuple_t normal = cross(plane->orient1,plane->orient2);
    normal = unitize(normal);
    plane->normal = normal;
    
    assert(scene->magic == SCENE_T);
    assert(ent->magic == ENTITY_T);
    
    // Make sure we get added to the list of objects.
    completeSceneObj(scene, ent);
}

/** dumpPlane **/
void dumpPlane(FILE *outFP, entity_t *ent) {
    
    // Do some shortcuts and sanity checks
    assert(ent->magic == ENTITY_T);
    sobj_t *obj = ent->entDerived;
    assert(obj->magic == SCENEOBJ_T);
    plane_t *plane = obj->sobjDerived;
    assert(plane->magic == PLANE_T);
    
    dumpSceneObj(outFP, ent);
    printTuple(outFP,  "   point:     ", plane->point);
    printTuple(outFP,  "   normal:    ", plane->normal);
}

