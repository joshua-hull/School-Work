#include "ray.h"

/** genRay **/
vector_t genRay(scene_t *scene, int column, int row) {
   vector_t direction;                       // Directior vector
   entity_t    *ent;
   window_t *window;

   assert(scene->magic == SCENE_T);
   ent = scene->window;
   window = ent->entDerived;
   assert(window->magic == WINDOW_T);

   /* Computer the pixel's real scene coordinates */
   direction.x = ((double)(column)/
      (double)(scene->picture->columns-1))*window->windowWidth;
   direction.x -= window->windowWidth/2.0;
   direction.y = ((double)(row)/
      (double)(scene->picture->rows-1))*window->windowHeight;
   direction.y -= window->windowHeight/2.0;
   direction.z = 0;

   /* And now construct a unit vector from the view point to the pixel */
   direction = ray(window->viewPoint, direction);
   direction = unitize(direction);
   return(direction);
} /* End genRay */

/** rayTrace **/
intensity_t rayTrace(scene_t *scene, point_t base, vector_t unitDir,
                 double total_dist, entity_t *self) {
    hitinfo_t *hitInfo = malloc(sizeof(hitinfo_t));

    entity_t *closestEntity = closest(scene,base,unitDir,self,hitInfo); 
    
    window_t *window = scene->window->entDerived;
    assert(window->magic == WINDOW_T);

    intensity_t lightingIntensity;
    
    // Default intensity
    intensity_t returnIntensity = {0,0,0};

    // If we didn't hit anything then return zeroed intensity.
    if (closestEntity == NULL) {
        free(hitInfo);
        return(returnIntensity);
    }

    sobj_t *closestObj = closestEntity->entDerived;
    assert(closestObj->magic == SCENEOBJ_T);
    assert(closestEntity->magic == ENTITY_T);
    
    // Don't really know why, we were told to do this.
    total_dist = total_dist + hitInfo->distance;
    
    // Scale the color of the closest object by the ambient light.
    returnIntensity.x = closestObj->color.r * window->ambient.x;
    returnIntensity.y = closestObj->color.g * window->ambient.y;
    returnIntensity.z = closestObj->color.b * window->ambient.z;
    
    // Calculate the lighting for non-ambient light sources

    lightingIntensity = lighting(scene, closestEntity, hitInfo);
    returnIntensity.x = returnIntensity.x + lightingIntensity.x;
    returnIntensity.y = returnIntensity.y + lightingIntensity.y;
    returnIntensity.z = returnIntensity.z + lightingIntensity.z;

    // Make it less then one.
    returnIntensity = scale(returnIntensity,1/255.0);

    // Have it fade into the distance.
    returnIntensity = scale(returnIntensity,1/total_dist);

    if (closestObj->reflective.x != 0 || closestObj->reflective.y != 0 || closestObj->reflective.z != 0) {
        vector_t V;
        vector_t U = unitize(scale(unitDir,-1.0));
        vector_t N = unitize( hitInfo->normal);
        V = N;
        V = scale(V,dot(U,N));
        V = scale(V,2);
        V = add(V,scale(U,-1.0));
        V = unitize(V);
    
        intensity_t reflections = rayTrace(scene,hitInfo->hitpoint,V,total_dist,closestEntity);

        reflections = (intensity_t){ reflections.x * closestObj->reflective.x,
                                     reflections.y * closestObj->reflective.y,
                                     reflections.z * closestObj->reflective.z};

        returnIntensity = add(returnIntensity,reflections);
    }   

    free(hitInfo);
    

    return(returnIntensity);

}

entity_t *closest(scene_t *scene, point_t base, vector_t unitDir, 
                  entity_t *self, hitinfo_t *hit) {
    
    iterator_t *sobjIter = newIterator(scene->sobjList);
    entity_t *returnEntity = NULL;
    hitinfo_t *closestHit = malloc(sizeof(hitinfo_t));
    double shortestDistance = -1.0;
    entity_t *entity;

    l_begin(sobjIter);
    
    while(l_hasnext(sobjIter) != 0) {
        entity = l_next(sobjIter);
        if(entity == self) {
            if(l_hasnext(sobjIter) != 0 ) {
                entity = l_next(sobjIter);
            } else {
                free(sobjIter);
                free(closestHit);
                return(returnEntity);
            }
        }
        assert(entity->magic == ENTITY_T);
        sobj_t *checkSobj = entity->entDerived;
        // If we are on the first object we've hit.
        if (shortestDistance == -1) {
            if(checkSobj->hit(entity,base,unitDir,closestHit) == 1) {
                shortestDistance = closestHit->distance;
                returnEntity = entity;
                *hit = *closestHit;
            }
        } else {
            if(checkSobj->hit(entity,base,unitDir,closestHit) == 1) {
                if(closestHit->distance < shortestDistance) {
                    shortestDistance = closestHit->distance;
                    returnEntity = entity;
                    *hit = *closestHit;    
                }
            }
        }
                
    }

    free(sobjIter);
    free(closestHit);

    return(returnEntity);
}
