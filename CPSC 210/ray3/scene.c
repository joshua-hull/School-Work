#include "ray.h"

/** getindex **/
int getindex(char *token, char *table[]) {
    int ndx = 0;
    while ((table[ndx] != NULL) && (strcasecmp(token, table[ndx]) != 0)) {
       ndx++;
    }
    return(ndx);
}

/** newScene **/
scene_t *newScene() {
   scene_t *new;
   new = malloc(sizeof(scene_t));
   new->magic = SCENE_T;
   new->sobjList = newList();
   new->lightList = newList();
   return(new);
}

/** loadScene **/
void loadScene(FILE *inFP, scene_t *scene) {
   char token[32];
   entity_t *obj;
   int  code;
   char *objectTypes[] = {"window",
                          "plane",
                          "sphere",
                          "pointlight",
                          "triangle",
                          "rectangle",
                          "ring",
                          "tiled",
                          NULL};

   /* Start processing scene description language */
   while (fscanf(inFP, "%31s", token) == 1) {
      code = getindex(token, objectTypes);
      switch (code) {
         case WINDOW:
            obj = newWindow(token, WINDOW);
            break;
         case PLANE:
            obj = newPlane(token, PLANE);
            break;
         case SPHERE:
            obj = newSphere(token, SPHERE);
            break;
         case LIGHT:
            obj = newPointLight(token,LIGHT);
            break;
         case TRIANGLE:
            obj = newTriangle(token,TRIANGLE);
            break;
         case RECTANGLE:
            obj = newRectangle(token, RECTANGLE);
            break;
        case RING:
            obj = newRing(token, RING);
            break;
        case TILED:
            obj = newTiled(token, TILED);
            break;
         default:
            fprintf(stderr, "Unknown token \"%s\"\n", token);
            exit(1);
      }
      /* Process the associated attributes */
      while ((fscanf(inFP, "%31s", token) == 1) && (strcmp(token, ";") != 0)) {
        obj->load(inFP, obj, token);
      }
      obj->complete(scene,obj);
//      fprintf(stderr, "Completed SDL processing for %s\n", obj->name);
//      fflush(stderr);

  // end while read objects
  }   
}

/** completeScene **/
void completeScene(scene_t *scene) {
   window_t *win;
   win = scene->window->entDerived;
   assert(win->magic == WINDOW_T);

   scene->picture = 
       newImage(win->pixelColumns, 
                win->pixelColumns*win->windowHeight/win->windowWidth, 255);
}

/** dumpScene **/
void dumpScene(FILE *outFP, scene_t *scene) {
   entity_t *obj;         // object_t pointer
   iterator_t *objitr;    // iterator to walk through list

   assert(scene->magic == SCENE_T);

   /* First dump the window data */
   dumpWindow(outFP, scene->window);

   /* Print the separately computed pixel rows value */
   fprintf(outFP, "   %-20s%-6d\n",
              "Pixel Height:", scene->picture->rows);

   /* Now dump the scene objects list */
   fprintf(outFP, "\nDUMP OF SCENE OBJECTS:\n");
   objitr = newIterator(scene->sobjList);
   while ((obj = l_next(objitr)) != NULL) {
      obj->dump(outFP,obj);
   }
   free(objitr);

   /* Next print light data */
   fprintf(outFP, "\nDUMP OF LIGHT LIST:\n");
   objitr = newIterator(scene->lightList);
   while ((obj = l_next(objitr)) != NULL) {
      obj->dump(outFP,obj);
   }
   free(objitr);
}
