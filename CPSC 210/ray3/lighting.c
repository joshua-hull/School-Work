#include "ray.h"

intensity_t lighting(scene_t *scene, entity_t *ent, hitinfo_t *hit) {

    assert(ent->magic == ENTITY_T);
    
    intensity_t returnIntensity = {0,0,0};
    intensity_t tmpIntensity;
    iterator_t *lightitr = newIterator(scene->lightList);
    entity_t *light;

    while((light = l_next(lightitr)) != NULL) {
        tmpIntensity = processPointLight(scene, ent, light, hit);
        returnIntensity.x = tmpIntensity.x + returnIntensity.x; 
        returnIntensity.y = tmpIntensity.y + returnIntensity.y;
        returnIntensity.z = tmpIntensity.z + returnIntensity.z;
    }
    free(lightitr);
    return(returnIntensity);
}
