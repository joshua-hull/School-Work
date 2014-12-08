#include "ray.h"

entity_t *newTiled(char *enttype, int code) {
    entity_t *returnEntity = newRectangle(enttype, code);
    assert(returnEntity->magic == ENTITY_T);

    sobj_t *planeObj = returnEntity->entDerived;
    assert(planeObj->magic == SCENEOBJ_T);

    plane_t *plane = planeObj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rect = plane->planeDerived;
    assert(rect->magic == RECTANGLE_T);
        

    tiled_t *tiled = malloc(sizeof(tiled_t));
    tiled->magic = TILED_T;

    rect->rectDerived = tiled;
    
    
    returnEntity->load = loadTiled;
    returnEntity->complete = completeTiled;
    returnEntity->dump = dumpTiled;

    tiled->color = (pixel_t) {255,0,0};
    tiled->diffuse = (intensity_t) {1,1,1};
    tiled->reflective = (intensity_t) {0,0,0};

    tiled->color2 = (pixel_t) {0,0,255};
    tiled->diffuse2 = (intensity_t) {1,1,1};
    tiled->reflective2 = (intensity_t) {0,0,0};

    tiled->width = 1;
    tiled->height = 1;


    
    ((sobj_t *)(returnEntity->entDerived))->hit = hitTiled;

    return(returnEntity);
}
void loadTiled(FILE *inFP, entity_t *ent, char *attribute) {
    char *attributes[]={"color2","diffuse2","reflective2","tilewidth","tileheight",NULL};
    int ndx;

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);
    
    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rect = plane->planeDerived;
    assert(rect->magic == RECTANGLE_T);

    tiled_t *tiled = rect->rectDerived;
    assert(tiled->magic = TILED_T);

    ndx = getindex(attribute, attributes);
    
    switch(ndx) {
        case 0:
            tiled->color2 = readColor(inFP, "Could not read color2.");
            break;
        case 1:
            tiled->diffuse2 = readTuple(inFP, "Could not read diffuse2.");
            break;
        case 2:
            tiled->reflective2 = readTuple(inFP, "Could not read reflective2.");
            break;
        case 3:
            tiled->width = readDouble(inFP, "Could not read tilewidth.");
            break;
        case 4:
            tiled->height = readDouble(inFP, "Could not read tileheight.");
            break;
        default:
            loadRectangle(inFP,ent,attribute);
            break;
    }
}
void completeTiled(scene_t *scene, entity_t *ent) {

    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rect = plane->planeDerived;
    assert(rect->magic == RECTANGLE_T);

    tiled_t *tiled = rect->rectDerived;
    assert(tiled->magic == TILED_T);

    completeRectangle(scene,ent);       

    tiled->color = planeobj->color;
    tiled->reflective = planeobj->reflective;
    tiled->diffuse = planeobj->diffuse; 
}
void dumpTiled(FILE *outFP, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);    

    dumpRectangle(outFP, ent);
    
    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rect = plane->planeDerived;
    assert(rect->magic == RECTANGLE_T);

    tiled_t *tiled = rect->rectDerived;
    assert(tiled->magic == TILED_T);

    fprintf(outFP,"   tilewidth:   %7.4lf\n",tiled->width);
    fprintf(outFP,"   tileheiight: %7.4lf\n",tiled->height);
    fprintf(outFP, "   color:        %6d   %6d   %6d\n", 
                                    tiled->color2.r, 
                                    tiled->color2.g,
                                    tiled->color2.b);

    printTuple(outFP, "    reflective2:    ",tiled->reflective2);
    printTuple(outFP, "    diffuse2:       ",tiled->diffuse2);
}
int hitTiled(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    
    assert(ent->magic == ENTITY_T);

    sobj_t *planeobj = ent->entDerived;
    assert(planeobj->magic == SCENEOBJ_T);

    plane_t *plane = planeobj->sobjDerived;
    assert(plane->magic == PLANE_T);

    rectangle_t *rect = plane->planeDerived;
    assert(rect->magic == RECTANGLE_T);

    tiled_t *tiled = rect->rectDerived;
    assert(tiled->magic == TILED_T);

    if (hitRectangle(ent, base, dir, hit) == 1) { 
        vector_t widthRay = ray(rect->edges[0].point,rect->edges[1].point);
        vector_t heightRay = ray(rect->edges[0].point,rect->edges[3].point);
        vector_t pointRay = ray(rect->edges[0].point,hit->hitpoint);

        vector_t widthProjection = project(widthRay,pointRay);
        vector_t heightProjection = project(heightRay,pointRay);

        int width = length(widthProjection)/tiled->width;
        int height = length(heightProjection)/tiled->height;

        int sum = width + height;

        if (sum % 2 == 0) {
            planeobj->color = tiled->color;
            planeobj->diffuse = tiled->diffuse;
            planeobj->reflective = tiled->reflective;
        } else {
            planeobj->color = tiled->color2;
            planeobj->diffuse = tiled->diffuse2;
            planeobj->reflective = tiled->reflective2;
        }
        return(1);
    } else {
        return(0);
    }   
}
