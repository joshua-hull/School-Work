/* Lab 8 unit test driver for hitPlane() function */

#include "ray.h"

/* Test planes */
plane_t pln[] = {
          {
             /* Plane 1 [HIT]: A back wall plane  parallel to screen */
             magic:  PLANE_T,
             point:  {0.0, 0.0, -5.0},
             normal: {0.0, 0.0, 1.0},
          },
          {
             /* Plane 2 [MISS]: A plane perpendicular to screen  */
             magic:  PLANE_T,
             point:  {0.0, 1.0, -5.0},
             normal: {0.0, 1.0, 0.0},
          },
          {
             /* Plane 3 [MISS]: A plane behind the viewpoint  */
             magic:  PLANE_T,
             point:  {0.0, 0.0, 5.0},
             normal: {0.0, 0.0, 1.0},
          },
          {
             /* Plane 4 [MISS]: A plane between the viewpoint and screen */
             magic:  PLANE_T,
             point:  {0.0, 0.0, 2.0},
             normal: {0.0, 0.0, 1.0},
          },
          {
             /* Plane 5 [HIT]: A slanted good plane */
             magic:  PLANE_T,
             point:  {0.0, -2.233 , -5.566},
             normal: {0.0, 0.5, 0.5},
          }
};


/* Test object */
sobj_t sobj;
    
entity_t   ent =
{
   magic:   ENTITY_T,
   name:    "testobj",
   type:    "plane",
   code:    PLANE,
};


/* Test viewpoint and ray direction */
vector_t  ray_dir    = {0, 0, -1};

/** main **/
int main(int argc, char *argv[])
{
    point_t   base;
    vector_t  unitdir;
    int       testNdx;
    plane_t   *planeData;
    hitinfo_t hitinfo;
    int       hitflag;

    if (argc != 4) {
       fprintf(stderr, "Usage: ./lab8 x y z\n");
       fprintf(stderr, "  where x, y and z are viewpoint coordinates\n");
       exit(1);
    }
    base.x = atof(argv[1]);
    base.y = atof(argv[2]);
    base.z = atof(argv[3]);

    sobj.magic = SCENEOBJ_T;
    ent.entDerived = &sobj;

    unitdir = unitize(ray_dir);

    for (testNdx=0; testNdx<5; testNdx++) {
        /* Set test plane */
        planeData = &pln[testNdx];
        sobj.sobjDerived = planeData;
        planeData->normal = unitize(planeData->normal);

        hitflag = hitPlane(&ent, base, unitdir, &hitinfo);

        fprintf(stderr, "\nTest %d\n", testNdx+1);
        if (!hitflag) {
           fprintf(stderr, "   MISS\n");
        }
        else {
           fprintf(stderr, "   HIT\n");
           fprintf(stderr, "   Returned distance:  %8.4lf\n", 
                           hitinfo.distance);
           printTuple(stderr,"   Coordinates at hit:", hitinfo.hitpoint);
           printTuple(stderr,"   Plane normal:      ", planeData->normal);
           printTuple(stderr,"   Normal vec at hit: ", hitinfo.normal);
        }
    }

    exit(0);
}
