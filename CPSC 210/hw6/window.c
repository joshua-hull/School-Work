/* window.c */

#include "ray.h"

/** newWindow **/
entity_t *newWindow(char *enttype, int code) { 
   /** STUBBED VERSION -- not a part of homework 6.
       Test version only, not suitable for homework 5 **/
   static window_t testwindow = {
      WINDOW_T,
      7, 9,
      450,
      (intensity_t){2, 2, 2},
      (point_t){1, 1, 5}};
   static entity_t testentity = {
      ENTITY_T,
      "testwindow",
      "window",
      WINDOW,
      &testwindow};
   return(&testentity);
}

/** loadWindow **/
void loadWindow(FILE *inFP, entity_t *ent, char *token) {
   /** STUBBED -- not a part of homework 6 **/
}

/** completeWindow **/
void completeWindow(scene_t *scene, entity_t *window) {
   /** STUBBED -- not a part of homework 6 **/
}

/** dumpWindow **/
void dumpWindow(FILE *out, entity_t *ent) {
   /** STUBBED -- not a part of homework 6 **/
   fprintf(out, "   dumpWindow stubbed -- not part of homework 6\n");
}

