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
         default:
            fprintf(stderr, "Unknown token \"%s\"\n", token);
            exit(1);
      }

      /* Process the associated attributes */
      while ((fscanf(inFP, "%31s", token) == 1) && (strcmp(token, ";") != 0)) {
         switch (obj->code) {
            case WINDOW:
               loadWindow(inFP, obj, token);
               break;
            case PLANE:
               loadPlane(inFP, obj, token);
               break;
            case SPHERE:
               loadSphere(inFP, obj, token);
               break;
            default:
               fprintf(stderr, "Oops -- unknown object type \"%d\"\n", 
                         obj->code);
               exit(1);
         }
      } // end while read attributes

      switch (obj->code) {
         case WINDOW:
            completeWindow(scene, obj);
            break;
         case PLANE:
            completePlane(scene, obj);
            break;
         case SPHERE:
            completeSphere(scene, obj);
            break;
         default:
            abort();  // Should never be able to get here!
      }
      fprintf(stderr, "Completed SDL processing for %s\n", obj->name);
      fflush(stderr);

   } // end while read objects
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
      switch(obj->code) {
         case WINDOW:
              dumpWindow(outFP, obj);
              break;
         case PLANE:
              dumpPlane(outFP, obj);
              break;
         case SPHERE:
              dumpSphere(outFP, obj);
              break;
         default: 
              fprintf(outFP, "Bad type while processing scene list\n");
              exit(1);
      }
   }
   free(objitr);

   /* Next print light data */
   fprintf(outFP, "\nDUMP OF LIGHT LIST:\n");
   objitr = newIterator(scene->lightList);
   /***** To be added later *****/
   free(objitr);
}
