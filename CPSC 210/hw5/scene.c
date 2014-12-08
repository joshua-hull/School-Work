// CP SC 210: hw5: scene.c (c) 2012 Joshua Hull
#include "ray.h"

/** newScene **/
scene_t *newScene() {
    
    // Allocate space for a scene_t
    scene_t *returnScene = malloc(sizeof(scene_t));
    
    // Initialize lists in the scene_t
    returnScene->sobjList = newList();
    returnScene->lightList = newList();
    
    // Set the magic number for the scene_t
    returnScene->magic = SCENE_T;
    
    // Return the scene_t
    return(returnScene);
}

/** completeScene **/
void completeScene(scene_t *scene) {
    
    // Create new pointer to make job easier
    window_t *window = scene->window->entDerived;
    
    // Make sure we really have a window_t at the other end
    assert(window->magic == WINDOW_T);

    
    // Shorten the typing we have to do by retrieving fields of
    // the window_t
    float windowWidth = window->windowWidth;
    float windowHeight = window->windowHeight;
    int columns = window->pixelColumns;
    
    // Calculate the number of rows the image has to have
    int rows = (windowHeight/windowWidth)*columns;
    
    // Create a new image image based on the info we were given
    // and point to it withing the scene_t structure.
    scene->picture = newImage(columns,rows,255);
}

/** dumpScene **/
void dumpScene(FILE *outFP, scene_t *scene) {
   assert(scene->magic == SCENE_T);
   /* First dump the window data */
   dumpWindow(outFP, scene->window);

   /* And then the computed pixel "rows" for the picture */
   fprintf(outFP, "   %-20s%-6d\n",
              "Pixel Height:", scene->picture->rows);

   /* Rest stubbed for now -- NOT part of homework 5 */
}
