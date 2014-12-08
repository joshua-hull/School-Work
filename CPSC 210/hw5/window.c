/* window.c */

#include "ray.h"

/** newWindow **/
entity_t *newWindow(char *enttype, int code) {
    
    // Create the entity that we are going to return.
    entity_t *returnEntity= newEntity(enttype,code);
    
    // Make some new space for what the entity is going to point
    // to and point to it. I this case, a window_t.
    returnEntity->entDerived = malloc(sizeof(window_t));
    
    // Grab a new pointer to make typing easier.
    window_t *window = returnEntity->entDerived;
    
    //Set the magic for the window_t struct and other default
    // values.
    window->magic = WINDOW_T;
    window->windowWidth = 6.0;
    window->windowHeight = 6.0;
    window->pixelColumns = 500;
    window->ambient = (intensity_t){1,1,1};
    window->viewPoint = (point_t){0.0,0.0,6.0};
    
    // Return the entity we created in the begining with the
    // new window_t with all the defaults.
    return(returnEntity);
}

/** completeWindow **/
void completeWindow(scene_t *scene, entity_t *window) {
    
    // Uhh, did this really need it's own function?
    scene->window = window;
}

/** dumpWindow **/
void dumpWindow(FILE *out, entity_t *ent) {
    
    // First dump the entity out.
    dumpEntity(out,ent);
    
    // Make a new pointer to make typing easier.
    window_t *window = ent->entDerived;
    
    // Make sure we really have a window_t at the other end.
    assert(window->magic == WINDOW_T);
    
    // Make some new pointers to make typing easier.
    point_t viewPoint = window->viewPoint;
    intensity_t ambient = window->ambient;
    
    // Print out the stuff we want all pretty and such.
    fprintf(out,"   Pixel Width:        %d\n",window->pixelColumns);
    fprintf(out,"   World Width:        %.1f\n",window->windowWidth);
    fprintf(out,"   World Height:       %.1f\n",window->windowHeight);
    fprintf(out,"   viewPoint:          %.1f     %.1f     %.1f\n",viewPoint.x,viewPoint.y,viewPoint.z);
    fprintf(out,"   ambient intensity:  %.1f     %.1f     %.1f\n",ambient.x,ambient.y,ambient.z);
}
