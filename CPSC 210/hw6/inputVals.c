/** CP SC 210

    Input procedures

    int readInt(FILE *infp, char *errmsg)
        Read a single integer from the input stream infp.  Print
        errmsg on error and terminate.

    pixel_t readColor(FILE *infp, char *errmsg) {
        Read pixel values from the input stream infp.  Print
        errmsg on error and terminate.

**/

#include "ray.h"

/** readInt **/
int readInt(FILE *infp, char *errmsg) {
    int result;
    if (fscanf(infp, "%d", &result) != 1) {
       /* Error exit */
       fprintf(stderr,"%s\n", errmsg);
       exit(1);
    }
    return(result);
}

/** readDouble **/
double readDouble(FILE *infp, char *errmsg) {
    double result;
    if (fscanf(infp, "%lf", &result) != 1) {
       /* Error exit */
       fprintf(stderr,"%s\n", errmsg);
       exit(1);
    }
    return(result);
}

/** readColor **/
pixel_t readColor(FILE *infp, char *errmsg) {
    pixel_t result;
    if (fscanf(infp, "%hhd %hhd %hhd", &result.r, &result.g, &result.b) != 3) {
       /* Error exit */
       fprintf(stderr,"%s\n", errmsg);
       exit(1);
    }
    return(result);
}

/** readTriple **/
triple_t readTriple(FILE *infp, char *errmsg) {
   triple_t triple;
   if (tRead(infp, &triple) != 3) {
      fprintf(stderr, "%s\n", errmsg);
      exit(1);
   }
   return(triple);
}
