#ifndef WORLD_H
#define WORLD_H

/** world_t -- pointers to scene and window data **/
typedef struct world_type
{
   /** World data **/
   double worldWidth;      /* Screen width in world coordinates  */
   double worldHeight;     /* Screen height in world coordinates */
   point_t  viewPoint;     /* Viewpt Loc in world coords         */
   intensity_t ambient;    /* Ambient light level                */
   int pixelWidth;         /* Pixel columns in output image      */
   image_t *outputImage;   /* output image                       */

   /** Lists of objects in the virtual world **/ 
   list_t  *sceneList;       /* "Shape objects" list             */
   list_t  *lightList;       /* "Lights" list                    */
}  world_t;


/** world prototype statements **/
object_t *newWorld();
void     procattrWorld(FILE *inFP, object_t *world, char *token);
void     completeWorld(object_t **worldPtr, object_t *obj);
void     printWorld(FILE *out, object_t *world);


#endif
