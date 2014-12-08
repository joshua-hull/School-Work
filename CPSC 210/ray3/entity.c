#include "ray.h"

entity_t *newEntity(char *type, int code) {
   entity_t *new = (entity_t *)malloc(sizeof(entity_t));

   assert(new != NULL);

   new->type =  strdup(type);
   new->code =  code;
   new->name =  "default";
   new->magic = ENTITY_T;

   return(new);
}

void loadEntity(FILE *inFP, entity_t *ent, char *token) {
   int count;
   char inName[32];

   assert(ent->magic == ENTITY_T);
   if (strcasecmp(token, "name") == 0) {
      /* Read the entect's name */
      count = fscanf(inFP, "%31s", inName);
      assert(count==1);
      ent->name = strdup(inName);
   }
   else {
      fprintf(stderr, "Bad attribute \"%s\"\n", token);
      exit(1);
   }
}

void dumpEntity(FILE *outFP, entity_t *ent) {

   fprintf(outFP, "\nType: %s\n", ent->type);
   fprintf(outFP, "   Name: %s\n", ent->name);
}
