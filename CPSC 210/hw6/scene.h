#ifndef SDL_H
#define SDL_H

#define SCENE_T  1933842

/** scene_t -- pointers to scene and window data **/
typedef struct scene
{
   int magic;                /* magic number                      */
   entity_t *window;         /* Window data                       */
   image_t  *picture;        /* output image                      */

   /** Lists of objects in the virtual world **/ 
   list_t  *sobjList;        /* scene objects list                */
   list_t  *lightList;       /* "Lights" list                     */
}  scene_t;

/** scene prototype statements **/
int     getindex(char *token, char *table[]);
scene_t *newScene();
void    loadScene(FILE *inFP, scene_t *scene);
void    completeScene(scene_t *scene);
void    dumpScene(FILE *outFP, scene_t *scene);

#endif
