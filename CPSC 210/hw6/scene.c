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
   /** STUBBED VERSION -- not a part of homework 6.
       Test version only -- not suitable for homework 5.
   **/
   static list_t testlist={NULL, NULL};
   static image_t testpicture={NULL, 888, 999, 255};
   static scene_t testscene={SCENE_T, NULL, &testpicture, &testlist, NULL};
   return(&testscene);
}

/** loadScene **/
void loadScene(FILE *inFP, scene_t *scene) {
    
    // List of 'shapes' that we know about.
    char *shapes[] = {"window","plane","sphere",";",NULL};
    char testString[100];
    int shapeIndex;
    entity_t *testEntity;
    // Scan until we hit the end of the file.
    while(fscanf(inFP,"%s",testString) != EOF) {
        // FInd out what shape we are dealing with.
        shapeIndex = getindex(testString,shapes);
        switch (shapeIndex) {
            case WINDOW:
                // Create a new window and process its attrbutes until we hit a
                // semi-colon.
                testEntity = newWindow(testString,WINDOW);
                fscanf(inFP,"%s",testString);
                while(strcmp(testString,";") != 0) {
                    loadWindow(inFP,testEntity,testString);
                    fscanf(inFP,"%s",testString);
                }
                completeWindow(scene,testEntity);
                break;
            case PLANE:
                // Create a new plane and process its attributes until we hit
                // a semi-colon
                testEntity = newPlane(testString,PLANE);
                fscanf(inFP,"%s",testString);
                while(strcmp(testString,";") != 0) {
                    loadPlane(inFP,testEntity,testString);
                    fscanf(inFP,"%s",testString);
                }
                completePlane(scene,testEntity);
                break;
            case SPHERE:
                // Create a new sphere and process its attributes untul we hit
                // a semi-colon
                testEntity = newSphere(testString,SPHERE);
                fscanf(inFP,"%s",testString);
                while(strcmp(testString,";") != 0) {
                    loadSphere(inFP,testEntity,testString);
                    fscanf(inFP,"%s",testString);
                }
                completeSphere(scene,testEntity);
                break;
            case 3:
                // If we get a semi-colon then just read in the next thing
                fscanf(inFP,"%s",testString);
                break;
            default:
                // We read in a shape we don't know about
                fprintf(stderr,"Unknown object type in SDL! Exiting!!\n");
                exit(0);
                break;
        }
       
    }
}

/** completeScene **/
void completeScene(scene_t *scene) {
   /** STUBBED  -- not a part of homework 6 **/
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
   /***** To be added later *****/
}
