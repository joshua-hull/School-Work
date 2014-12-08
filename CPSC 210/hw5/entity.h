#ifndef ENTITY_H
#define ENTITY_H

#define ENTITY_T 5321589

/** entity_t -- base object type **/
typedef struct entity {
   int     magic;       /* magic number             */
   char    *name;       /* left_wall, center_sphere */
   char    *type;       /* plane, sphere, ...       */
   int     code;        /* Code for object type     */

   void    *entDerived; /* Pointer to object derived from an entity_t */

} entity_t;

/** entity prototypes **/
entity_t *newEntity(char *enttype, int entcode);
void  loadEntity(FILE *inFP, entity_t *ent, char *token);
void  dumpEntity(FILE *out, entity_t *ent);

#endif
