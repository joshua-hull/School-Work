#ifndef WINDOW_H
#define WINDOW_H

#define WINDOW_T 3311657

/** window_t -- window data **/
typedef struct window
{
   int        magic;         /* Magic number                    */
   
   double     windowWidth;   /* Window width in world units     */
   double     windowHeight;  /* Window height in world units    */
   int        pixelColumns;  /* Window width in pixels          */
   intensity_t ambient;      /* Ambient lighting on scene       */     
   point_t    viewPoint;     /* Viewpoint coordinates           */
}  window_t;

/** sphere prototype statements **/
entity_t     *newWindow(char *objtype, int code);
void      loadWindow(FILE *inFP, entity_t *obj, char *token);
void      completeWindow(scene_t *scene, entity_t *obj);
void      dumpWindow(FILE *outFP, entity_t *ent);

#endif
