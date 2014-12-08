#ifndef SCENEOBJ_H
#define SCENEOBJ_H

#include "entity.h"

#define SCENEOBJ_T 4568892

/** sobj_t -- Visible scene objects **/
typedef struct sceneobj {
   int magic;                /* magic number                   */
   /* Surface data */
   pixel_t color;            /* surface color                  */
   intensity_t  diffuse;     /* light reflection               */
   intensity_t  reflective;  /* ray reflection                 */

   void    *sobjDerived;      /* Pointer to derived scene object data  */
} sobj_t; 


/** shape prototype statements **/
entity_t    *newSceneObj(char *type, int code);
void     loadSceneObj(FILE *inFP, entity_t *obj, char *token);
void     completeSceneObj(scene_t *scene, entity_t *obj);
void     dumpSceneObj(FILE *out, entity_t *obj);

#endif
