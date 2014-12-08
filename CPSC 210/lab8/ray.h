/* ray.h */
#ifndef RAY_H
#define RAY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <memory.h>
#include <assert.h>

/** object types **/
#define WINDOW  0
#define PLANE   1
#define SPHERE  2
#define LIGHT   3

/** Includes of data types and function prototypes for ray tracer object.

    NOTE: be VERY careful about the order of the includes.  An object
          type (typedef) MUST be declared before it can be used, including
          by another include.  
**/
#include "tuple.h"
/* For readability, equate other 3-tuple types to a tuple_t */
typedef tuple_t vector_t;
typedef tuple_t point_t;
typedef tuple_t intensity_t;

#include "image.h"
#include "entity.h"
#include "list.h"
#include "scene.h"
#include "sceneobj.h"

/** hitinfo_t **/
typedef struct hitinfo {
   point_t hitpoint;
   vector_t normal;
   double distance;
} hitinfo_t;

#include "plane.h"
   
#endif
