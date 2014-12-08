#ifndef RAYTRACE_H
#define RAYTRACE_H


/** Prototype statements **/
intensity_t rayTrace(scene_t *scene, point_t base, vector_t unitDir,
                 double total_dist, entity_t *self);
entity_t       *closest(scene_t *scene, point_t base, vector_t unitDir, 
                 entity_t *self, hitinfo_t *hit);
vector_t    genRay(scene_t *scene, int columnNdx, int rowIndx);

#endif
