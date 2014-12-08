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
    
    // Default intensity
    intensity_t returnIntensity = {0,0,0};

    // If we didn't hit anything then return zeroed intensity.
    if (closestEntity == NULL) {
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

    // Make it less then one.
    returnIntensity = scale(returnIntensity,1/255.0);

    // Have it fade into the distance.
    returnIntensity = scale(returnIntensity,1/total_dist);

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
        assert(entity->magic == ENTITY_T);
        // If we are on the first object we've hit.
        if (shortestDistance == -1) {
            // If we are dealing with a plane
            if(strcmp(entity->type,"plane") == 0 ) {
                // and we hit it
                if(hitPlane(entity,base,unitDir,closestHit) == 1) {
                    // Since it is the first hit we set it as the shortest distance so far.
                    shortestDistance = closestHit->distance;
                    returnEntity = entity;
                    *hit = *closestHit;
                }
            // Do the same thing with a sphere.
            } else if (strcmp(entity->type,"sphere") == 0) {
                if(hitSphere(entity,base,unitDir,closestHit) == 1) {
                    shortestDistance = closestHit->distance;
                    returnEntity = entity;
                    *hit = *closestHit;
                }
            }
        } else {
            if(strcmp(entity->type,"plane") == 0) {
                if(hitPlane(entity,base,unitDir,closestHit) == 1) {
                    // If this distnace is shorter then the previous shortest
                    if (closestHit->distance < shortestDistance) {
                        shortestDistance = closestHit->distance;
                        returnEntity = entity;
                        *hit = *closestHit;
                    }
                }
            } else if (strcmp(entity->type,"sphere") == 0) {
                if(hitSphere(entity,base,unitDir,closestHit) == 1) {
                    if (closestHit->distance < shortestDistance) {
                        shortestDistance = closestHit->distance;
                        returnEntity = entity;
                        *hit = *closestHit;
                    }
                }
            }
        }
                
    }

    return(returnEntity);
}
