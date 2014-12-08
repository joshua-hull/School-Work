#ifndef SCENE_H
#define SCENE_H

#define SCENE_T  1933842

/** scene_t -- pointers to scene and window data **/
typedef struct scene_type
{
   int magic;                /* magic number                     */
   /** Scene data **/
   double sceneWidth;      /* Screen width in world coordinates  */
   double sceneHeight;     /* Screen height in world coordinates */
   point_t  viewPoint;     /* Viewpt Loc in world coords         */
   intensity_t ambient;    /* Ambient light level                */
   int pixelWidth;         /* Pixel columns in output image      */
   int samples;            /* antialiasing random samples        */
   image_t *outputImage;   /* output image                       */

   /** Lists of objects in the virtual world **/ 
   list_t  *shapeList;       /* "Shape objects" list             */
   list_t  *lightList;       /* "Lights" list                    */
}  scene_t;


/** scene prototype statements **/
object_t *newScene();
void completeScene(object_t *scene);
void loadScene(FILE *inFP, object_t *scene, char *token);
void dumpScene(FILE *out, object_t *scene);


#endif
