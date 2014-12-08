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

void loadEntity(FILE *inFP, entity_t *ent, char *token) {
    char *namecmp = "name";
    char namePtr[100];
    // We are the last link in the chain. If we aren't processing a 'name'
    // then the program doens't know what this thing is.
    if(strcasecmp(token,namecmp) != 0) {
        fprintf(stderr,"Error proccessing SDL! Exiting!!");
        exit(1);
    }
    // Scan in what ever the name is.
    fscanf(inFP,"%s",namePtr);
    if (namePtr == NULL) {
        fprintf(stderr,"Error processing SDL! Exiting!!");
        exit(1);
    }
    // and assign it.
    ent->name = strdup(namePtr);
}

void dumpEntity(FILE *outFP, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   fprintf(outFP, "\nType: %s\n", ent->type);
   fprintf(outFP, "   Name: %s\n", ent->name);
}
