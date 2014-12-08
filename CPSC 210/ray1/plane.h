/* plane.h */
#ifndef PLANE_H
#define PLANE_H

#define PLANE_T 2338767

/** plane_t --  infinite plane data **/
typedef struct plane_type
{
   int      magic;        /* magic number                      */
   vector_t orient1;      /* Orientation 1 of plane            */
   vector_t orient2;      /* Orientation 2 of plane            */
   point_t  point;        /* Any point on surface              */
   vector_t normal;       /* Computed normal to plane          */
   void     *planeDerived;   /* Pointer to derived object data */
}  plane_t;

/** plane function prototype statements **/
entity_t     *newPlane(char *objtype, int code);
void      loadPlane(FILE *inFP, entity_t *obj, char *token);
void      completePlane(scene_t *scene, entity_t *obj);
void      dumpPlane(FILE *outFP, entity_t *obj);
int       hitPlane(entity_t *obj, point_t base, vector_t dir, hitinfo_t *hit);

#endif
