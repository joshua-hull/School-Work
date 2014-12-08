/** main.c 

    Main program for ray tracer

**/
#include "ray.h"

int main( int argc, char *argv[])
{
   scene_t *scene;
   FILE    *inFP;

   /* Open scene definition file */
   if (argc != 3) {
      fprintf(stderr, "Usage: ./ray mdl_file ppm_image_file\n");
      exit(1);
   }
   if ((inFP = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "ERROR: could not open sdl file \"%s\"\n", argv[1]);
      exit(1);
   }

   /* Load and dump scene data */
   scene = newScene();
   loadScene(inFP, scene);
   completeScene(scene);

   dumpScene(stderr, scene);

   /* Render the image */
   render(scene);

   /* And output the image to a PPM file */
   outImage(argv[2], scene->picture);

   exit(0);
}

