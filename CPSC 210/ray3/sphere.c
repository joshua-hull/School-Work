#include "ray.h"

/** newSphere **/
entity_t *newSphere(char *enttype, int code) {
   entity_t *ent;
   sphere_t *sphere;
   vector_t zero = {0, 0, 0};

   /* create new sphere structure */
   ent = newSceneObj(enttype, code);
   sphere = malloc(sizeof(sphere_t));
   assert(sphere != NULL);
   ((sobj_t *)(ent->entDerived))->sobjDerived = sphere;

   /* Set default values */
   sphere->center = zero;
   sphere->radius = 1;
   sphere->magic = SPHERE_T;

   ent->load = loadSphere;
   ent->dump = dumpSphere;
   ent->complete = completeSphere;

   ((sobj_t *)(ent->entDerived))->hit = hitSphere;

   return(ent);
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

/** hitSphere **/
int hitSphere(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit) {
    
    tuple_t V = base;
    tuple_t D = dir;

    sphere_t *sphere = ((sobj_t *)(ent->entDerived))->sobjDerived;
    assert(sphere->magic == SPHERE_T);
    tuple_t C = sphere->center;

    tuple_t V_Prime = {V.x-C.x,V.y-C.y,V.z-C.z};

    double a = dot(D,D);
    double b = 2*dot(V_Prime,D);
    double c = dot(V_Prime,V_Prime) - pow(sphere->radius,2);

    // If the sphere does get hit
    if ( (pow(b,2) - (4*a*c)) > 0) {
        double t = (-b - sqrt(pow(b,2) - 4*a*c))/(2*a);
        tuple_t H = add(V,scale(D,t));
        // Check to make sure it's not in front of the screen.
        if (H.z > 0 ) {
            return(0);
        }
        tuple_t N = add(H,scale(C,-1));
        N = unitize(N);
        hit->hitpoint = H;
        hit->normal = N;
        hit->distance = length(ray(base,H));
        return(1);
    } else {
        return(0);
    }
}
