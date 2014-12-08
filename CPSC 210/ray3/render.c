#include "ray.h"

/** render **/
void render(scene_t *scene) {
    image_t *picture = scene->picture;
    int rowNdx;
    int columnNdx;
    pixel_t *pixel = picture->image;

    // For each pixel in the image simply call makePixel.
    for (rowNdx = picture->rows - 1; rowNdx >= 0; rowNdx--) {
        for(columnNdx = 0; columnNdx < picture -> columns; columnNdx++) {
            *pixel = makePixel(scene,columnNdx,rowNdx);
            pixel++;
        } 
    }
}

/** makePixel **/
pixel_t makePixel(scene_t *scene, int colndx, int rowndx) {
    pixel_t returnPixel;
    intensity_t intensity;
    vector_t dir;
    window_t *window = scene->window->entDerived;
    assert(window->magic == WINDOW_T);

    // Get a ray from our viewpoint to the pixel and unitize it.
    dir = genRay(scene,colndx,rowndx);
    dir = unitize(dir);

    // Get the intensity for that pixel.
    intensity = rayTrace(scene,window->viewPoint,dir,0.0,NULL);
    
    // Make sure no intensity is above 1.
    if (intensity.x > 1.0) {
        intensity.x = 1.0;
    }
    if (intensity.y > 1.0) {
        intensity.y = 1.0;
    }
    if (intensity.z > 1.0) {
        intensity.z = 1.0;
    }
    
    // Scale up the intensity in each component.
    returnPixel.r = 255*intensity.x;
    returnPixel.g = 255*intensity.y;
    returnPixel.b = 255*intensity.z;
    
   return(returnPixel);
}
