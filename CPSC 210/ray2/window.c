/* window.c */

#include "ray.h"

/** newWindow **/
entity_t *newWindow(char *enttype, int code) { 
   entity_t *ent;
   window_t *window;

   /* create new window object */
   ent = newEntity(enttype, code);
   assert(ent->magic == ENTITY_T);
   window = malloc(sizeof(window_t));
   assert(window != NULL);
   ent->entDerived = window;

   /* Set the default values */
   window->windowWidth = 4;
   window->windowHeight = 4;
   window->viewPoint = (point_t) {0, 0, 5};
   window->ambient = (point_t) {1, 1, 1};
   window->pixelColumns = 600;

   /* Set the window_t's magic number */
   window->magic = WINDOW_T;
   
   ent->load = loadWindow;
   ent->complete = completeWindow;
   ent->dump = dumpWindow;

   return(ent);
} /* End newWindow */

/** loadWindow **/
void loadWindow(FILE *inFP, entity_t *ent, char *token) {
    char *attributes[] = {"windowwidth", "windowheight","columns","viewpoint","ambient", NULL};
    int ndx;
    assert(ent->magic == ENTITY_T);
    window_t *window = ent->entDerived;
    assert(window->magic == WINDOW_T);

    ndx = getindex(token, attributes);

    switch (ndx) {
        case 0:
            // windowWidth
            window->windowWidth = readDouble(inFP,"Could not read windowWidth");
            break;

        case 1:
            // windowHeight
            window->windowHeight = readDouble(inFP, "Could not read windowHeight");
            break;
    
        case 2:
            // pixelColumn
            window->pixelColumns = readInt(inFP, "Could not read pixelColumns");
            break;

        case 3:
            // viewPoint
            window->viewPoint = readTuple(inFP, "Could not read viewPoint");
            break;

        case 4:
            // ambient
            window->ambient = readTuple(inFP, "Could not read ambient");
            break;

        default:
            loadEntity(inFP, ent, token);
   }


}

/** completeWindow **/
void completeWindow(scene_t *scene, entity_t *window) {
   assert(window->magic == ENTITY_T);
   window_t *windowPtr = window->entDerived; // Pointer to window_t
   assert(windowPtr->magic == WINDOW_T);

   scene->window = window;
} /* end completeWindow */

/** dumpWindow **/
void dumpWindow(FILE *out, entity_t *ent) {
   assert(ent->magic == ENTITY_T);
   window_t *window = ent->entDerived;  // window_t pointer
   assert(window->magic == WINDOW_T);

   /* dump the object_t data */
   dumpEntity(out, ent);

   /* Now the window_t data */
   fprintf(out, "   %-20s%-6d\n", 
                                 "Pixel Width:",
                                 window->pixelColumns);
   fprintf(out, "   %-20s%-6.1lf\n", 
                                 "World Width",
                                 window -> windowWidth);
   fprintf(out, "   %-20s%-6.1lf\n", 
                                 "World Height",
                                 window -> windowHeight);
   fprintf(out, "   %-20s%-6.1lf%-6.1lf%-6.1lf \n",
                                "viewPoint",
                                (window -> viewPoint).x,
                                (window -> viewPoint).y, 
                                (window -> viewPoint).z );
   fprintf(out, "   %-20s%-6.1lf%-6.1f%-6.1lf\n", 
                                "ambient intensity:",
                                (window -> ambient).x, 
                                (window -> ambient).y, 
                                (window -> ambient).z );
} /* End dumpWindow */

