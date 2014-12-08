
/** main.c **/
#include "ray.h"

int main( int argc, char *argv[])
{
   scene_t *scene;
   FILE    *inFP;

   /* Open scene definition file */
   if (argc != 2) {
      fprintf(stderr, "Usage: ./hw6 sdl_file\n");
      exit(1);
   }
   assert((inFP = fopen(argv[1], "r")) != NULL);


   /* Load and dump scene data */
   scene = newScene();
   loadScene(inFP, scene);
   completeScene(scene);

   dumpScene(stderr, scene);

   return(0);
}

