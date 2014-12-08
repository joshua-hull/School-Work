#include "ray.h"

/** genRay **/
vector_t genRay(scene_t *scene, int column, int row) {
    
    // Make a place holder for where we are in the virtual world.
    tuple_t worldCoord;
    
    // Make a pointer to make typing easier.
    window_t *window = scene->window->entDerived;
    
    // Make sure the other person is on the other end of the line
    assert(window->magic == WINDOW_T);
    
    // Calcualte the x coordinate.
    worldCoord.x = ((double)(column)/(double)(scene->picture->columns-1))*window->windowWidth;
    // Adjust for the center of the world being in the middle of the screen.
    worldCoord.x -= window->windowWidth/2.0;
    
    // Calcualte the y coordinate.
    worldCoord.y = ((double)(row)/(double)(scene->picture->rows-1))*window->windowHeight;
    // Adjust for the center of the world being in the middle of the screen.
    worldCoord.y -= window->windowHeight/2.0;
    
    // All pixels live on the x-y plane.
    worldCoord.z = 0;
    
    // Create a vector from the viewpoint to out pixel
    tuple_t vector = ray((tuple_t)window->viewPoint,worldCoord);
    // Make that vector a unit vector so we can return it.
    tuple_t unitVector = unitize(vector);
    
    return((vector_t)unitVector);
}

