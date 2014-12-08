#include "ray.h"

/** newSphere **/
entity_t *newSphere(char *enttype, int code) {
    
    // Do some shortcust and sanity checks.
    entity_t *returnEntity = newSceneObj(enttype,code);
    sobj_t *obj = returnEntity->entDerived;
    assert(obj->magic == SCENEOBJ_T);
    
    // Create some sphere-specific stuff and return the new entity.
    obj->sobjDerived = (sphere_t *) malloc(sizeof(sphere_t));
    sphere_t *sphere = obj->sobjDerived;
    sphere->magic = SPHERE_T;
    return(returnEntity);
}

/** loadSphere **/
void loadSphere(FILE *inFP, entity_t *ent, char *attribute) {
   char *attributes[] = {"center", "radius", NULL};
   int ndx;
   assert(ent->magic == ENTITY_T);
   sobj_t *obj = ent->entDerived;
   assert(obj->magic == SCENEOBJ_T);
   sphere_t *sphere = obj->sobjDerived;
   assert(sphere->magic == SPHERE_T);

   ndx = getindex(attribute, attributes);

   switch (ndx) {
      case 0:
         /** center **/
         sphere->center = readTuple(inFP, 
              "Could not read sphere center coordinates");
         break;

      case 1:
         /** radius **/
         sphere->radius = readDouble(inFP, "Could not read sphere radius");
         break;

      default:
         loadSceneObj(inFP, ent, attribute);
   }


};

/** completeShere **/
void completeSphere(scene_t *scene, entity_t *ent) {
     assert(scene->magic == SCENE_T);
     assert(ent->magic == ENTITY_T);
     completeSceneObj(scene, ent);
}

/** dumpSphere **/
void dumpSphere(FILE *out, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   sobj_t *obj = ent->entDerived;
   assert(obj->magic == SCENEOBJ_T);
   sphere_t *sphere = obj->sobjDerived;
   assert(sphere->magic == SPHERE_T);

   dumpSceneObj(out, ent);
   printTuple(out,  "   center:    ", sphere->center);
   fprintf(out,     "   radius:     %8.4lf\n",
            sphere->radius);
}

