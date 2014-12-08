/* plane.c */

#include "ray.h"

/** newPlane **/
entity_t *newPlane(char *enttype, int code) {
   entity_t *ent;
   plane_t *plane;
   vector_t zero = {0, 0, 0};

   /* create new plane structure */
   ent = newSceneObj(enttype, code);
   plane = malloc(sizeof(plane_t));
   assert(plane != NULL);
   ((sobj_t *)(ent->entDerived))->sobjDerived = plane;

   /* Set default values */
   plane->point = zero;
   plane->orient1 = (vector_t){1, 0, 0};
   plane->orient2 = (vector_t){0, 1, 0};

   plane->magic = PLANE_T;

   ent->load = loadPlane;
   ent->dump = dumpPlane;
   ent->complete = completePlane;

   ((sobj_t *)(ent->entDerived))->hit = hitPlane;

   return(ent);
}

/** loadPlane **/
void loadPlane(FILE *inFP, entity_t *ent, char *attribute) {
   char *attributes[] = {"point", "orient1", "orient2", NULL};
   int ndx;
   assert(ent->magic == ENTITY_T);
   sobj_t *sobj = ent->entDerived;
   assert(sobj->magic == SCENEOBJ_T);
   plane_t *plane = sobj->sobjDerived;
   assert(plane->magic == PLANE_T);

   ndx = getindex(attribute, attributes);

   switch (ndx) {
      case 0:
         /* point */
         plane->point = readTuple(inFP, "Could not read plane point");
         break;

      case 1:
         /* orient1 */
         plane->orient1 = readTuple(inFP, "Could not read plane orient1");
         break;

      case 2:
         /* orient1 */
         plane->orient2 = readTuple(inFP, "Could not read plane orient2");
         break;

      default:
         loadSceneObj(inFP, ent, attribute);
   }
}

/** completePlane **/
void completePlane(scene_t *scene, entity_t *ent) {
     sobj_t *sobj;
     plane_t *plane;
     assert(scene->magic == SCENE_T);
     assert(ent->magic == ENTITY_T);
     sobj = ent->entDerived;
     assert(sobj->magic == SCENEOBJ_T);
     plane = sobj->sobjDerived;
     assert(plane->magic == PLANE_T);
     plane->normal = unitize(cross(plane->orient1, plane->orient2));
     completeSceneObj(scene, ent);
}

/** dumpPlane **/
void dumpPlane(FILE *outFP, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   sobj_t *sobj = ent->entDerived;
   assert(sobj->magic == SCENEOBJ_T);
   plane_t *plane = sobj->sobjDerived;
   assert(plane->magic == PLANE_T);

   dumpSceneObj(outFP, ent);

   printTuple(outFP,  "   point:     ", plane->point);
   printTuple(outFP,  "   orient1:   ", plane->orient1);
   printTuple(outFP,  "   orient2:   ", plane->orient2);
   printTuple(outFP,  "   normal:    ", plane->normal);
}


int hitPlane(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    assert(ent->magic == ENTITY_T);
    sobj_t *sobj =  ent->entDerived;     
    assert(sobj->magic == SCENEOBJ_T);
    plane_t *planePtr = sobj->sobjDerived;
    assert(planePtr->magic == PLANE_T);

    point_t  Q = planePtr->point;      /* Point data      */
    vector_t N = planePtr->normal;     /* Normal data     */
    vector_t D = dir;                  /* Direction vector*/
    point_t  V = base;                 /* Base coordinates*/
    point_t  H;                        /* Hit point                 */
    double   t;                        /* Distance                  */

    if (dot(N, D) == 0)
        return 0; // parallel
    
    t = (dot(N, Q) - dot(N, V))/
           dot(N, D);
    if (t < 0)
        return 0; // behind me
    
    H = scale(D, t); 
    H = add(H, V);
    if (H.z > 0)
        return 0; // between the "screen" and me
    
    hit->hitpoint = H;
    hit->normal = unitize(N);
    hit->distance = t;
    /* Adjust the normal depending on whether ray hit from front
       or back of plane */
    if (dot(D, hit->normal) > 0) {
       hit->normal = scale(hit->normal, -1);
    }

    return 1;
}

