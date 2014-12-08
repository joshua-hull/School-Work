#ifndef SPHERE_H
#define SPHERE_H

#define SPHERE_T 9133224

/** sphere_t -- sphere data **/
typedef struct sphere_type
{
   int        magic;      /* Magic number                    */
   point_t    center;     /* Location of the center          */
   double     radius;     /* distance from center to surface */
   void       *derivedSphere; /* Pointer to derived object data  */
}  sphere_t;

/** sphere prototype statements **/
entity_t  *newSphere(char *enttype, int code);
void   loadSphere(FILE *inFP, entity_t *ent, char *token);
void   completeSphere(scene_t *scene, entity_t *ent);
void   dumpSphere(FILE *outFP, entity_t *ent);
int    hitSphere(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit);

#endif
