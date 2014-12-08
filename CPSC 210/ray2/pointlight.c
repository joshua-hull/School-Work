#include "ray.h"

entity_t *newPointLight(char *enttype, int code) {
    entity_t *ent;
    pointlight_t *pointlight = malloc(sizeof(pointlight_t));
    assert(pointlight != NULL);
    
    ent = newEntity(enttype, code);
    ent->entDerived = pointlight;
    
    pointlight->color = (pixel_t){255,255,255};
    pointlight->brightness = 1.0;
    pointlight->center = (point_t){0,0,0};
    
    ent->load = loadPointLight;
    ent->complete = completePointLight;
    ent->dump = dumpPointLight;

    pointlight->magic = POINTLIGHT_T;

    return(ent);
}

void loadPointLight(FILE *inFP, entity_t *ent, char *token){
    char *attributes[] = {"color","brightness","center",NULL};
    int ndx;
    
    assert(ent->magic == ENTITY_T);
    pointlight_t *pointlight = ent->entDerived;
    assert(pointlight->magic == POINTLIGHT_T);
    
    ndx = getindex(token,attributes);
    
    switch(ndx) {
        case 0:
            pointlight->color = readColor(inFP, "Could not read light color");
            break;
        case 1:
            pointlight->brightness = readDouble(inFP, "Could not read light brightness");
            break;
        case 2:
            pointlight->center = readTuple(inFP, "Could not read light center");
            break;
        default:
            loadEntity(inFP, ent, token);
            break;
        }
}

void completePointLight(scene_t *scene, entity_t *ent) {
    l_add(scene->lightList, ent);
}

void dumpPointLight(FILE *outFP, entity_t *ent) {
    
    assert(ent->magic == ENTITY_T);
    
    pointlight_t *pointlight = ent->entDerived;
    
    dumpEntity(outFP, ent);
    fprintf(outFP, "   color:        %6d   %6d   %6d\n",pointlight->color.r,pointlight->color.g,pointlight->color.b);
    fprintf(outFP, "   brightness: %8.4lf\n",pointlight->brightness);
    printTuple(outFP, "   center:    ",pointlight->center);
}

intensity_t processPointLight(scene_t *scene, entity_t *ent, entity_t *light, hitinfo_t *hit) {
    
    assert(ent->magic == ENTITY_T);

    tuple_t surfaceNormal = unitize(hit->normal);
    tuple_t testRay = ray(hit->hitpoint,((pointlight_t *)(light->entDerived))->center);
    testRay = unitize(testRay);
    intensity_t returnIntensity = {0,0,0};
    hitinfo_t *occlude = malloc(sizeof(hitinfo_t));

    pointlight_t *pointlight = light->entDerived;
    assert(pointlight->magic == POINTLIGHT_T);

    sobj_t *obj = ent->entDerived;
    assert(obj->magic == SCENEOBJ_T);

    intensity_t diffuse = obj->diffuse;
    intensity_t intensity = {pointlight->color.r * pointlight->brightness,pointlight->color.g*pointlight->brightness,pointlight->color.b*pointlight->brightness};
    double lightDistance = length(ray(pointlight->center,hit->hitpoint));

    if( dot(testRay,surfaceNormal) > 0 ) {
        entity_t *occludingObject = closest(scene,pointlight->center,scale(testRay, -1.0),ent, occlude);
        free(occlude);
        if(occludingObject == ent){ 
            returnIntensity.x = diffuse.x * intensity.x * dot(testRay,surfaceNormal); 
            returnIntensity.y = diffuse.y*intensity.y * dot(testRay,surfaceNormal);
            returnIntensity.z = diffuse.z*intensity.z * dot(testRay,surfaceNormal);
            returnIntensity = scale(returnIntensity,1.0/lightDistance);
            return(returnIntensity);
        } else {
            return(returnIntensity);
        }
    } else {
        free(occlude);
        return(returnIntensity);    
    }
}
