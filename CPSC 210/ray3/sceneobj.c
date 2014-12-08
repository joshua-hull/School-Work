/* shape.c */

#include "ray.h"

/** newSceneObj **/
entity_t *newSceneObj(char *type, int code) {
   entity_t *ent;                   /* Pointer to new entity_t */
   sobj_t *new;                    /* Pointer to new sobj_t  */
   pixel_t white = {255, 255, 255}; /* Default color           */
   intensity_t zero = {0, 0, 0};    /* Default diffuse/reflective values */

   /* create new sceneObject structure */
   ent = newEntity(type, code);
   new = malloc(sizeof(sobj_t));
   assert(new != NULL);
   ent->entDerived = new;

   /* Set default values for a shape */
   new->color = white;
   new->diffuse = zero;
   new->reflective = zero;
   new->magic = SCENEOBJ_T;

   return(ent);
}


/** loadSceneObj **/
void loadSceneObj(FILE *inFP, entity_t *ent, char *attribute) {
   /* Attributes recognized by loadSceneObj */
   char *attributes[] = {"color", "diffuse", "reflective", NULL};
   int attributeNdx;    /* Index from attributes array that matches attribute */

   assert(ent->magic == ENTITY_T);
   sobj_t *obj = ent->entDerived;      /* Pointer to object's sobj_t       */
   assert(obj->magic == SCENEOBJ_T);

   /* Get the index of the attribute */
   attributeNdx = getindex(attribute, attributes);

   /* Process the attribute */
   switch (attributeNdx) {
      case 0:
         /** color  attribute **/
         obj->color = readColor(inFP, "Could not read color values");
         break;

      case 1:
         /** diffuse attribute **/
         obj->diffuse = readTuple(inFP, "Could not read diffuse values");
         break;

      case 2:
         /** reflective attribute **/
         obj->reflective = readTuple(inFP, 
             "Could not read reflective vaules");
         break;

      default:
         loadEntity(inFP, ent, attribute);
   }

}

/** completeSceneObj **/
void completeSceneObj(scene_t *scene, entity_t *ent) {
   assert(scene->magic == SCENE_T);

   /* Just add new shape to sceneListList linked list */
   l_add(scene->sobjList, ent);
}

/** dumpSceneObj **/
void dumpSceneObj(FILE *out, entity_t *ent) {
   assert(ent->magic == ENTITY_T);

   sobj_t *obj = ent->entDerived;
   assert(obj->magic == SCENEOBJ_T);

   /* First dump the entity_t data */
   dumpEntity(out, ent);

   /* And now the sobj_t specific data */
   fprintf(out, "   color:        %6d   %6d   %6d\n", 
                                    obj->color.r, 
                                    obj->color.g,
                                    obj->color.b);
   printTuple(out,  "   diffuse:   ", obj->diffuse);
   printTuple(out,  "   reflective:", obj->reflective);
}

