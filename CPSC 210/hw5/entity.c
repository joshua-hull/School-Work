#include "ray.h"

entity_t *newEntity(char *type, int code) {
   entity_t *new = (entity_t *)malloc(sizeof(entity_t));

   assert(new != NULL);

   new->type =  strdup(type);
   new->code = code;
   new->name = strdup("default");
   new->magic = ENTITY_T;

   return(new);
}

void dumpEntity(FILE *outFP, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   fprintf(outFP, "\nType: %s\n", ent->type);
   fprintf(outFP, "   Name: %s\n", ent->name);
}
