#include "ray.h"

int main() {
    scene_t *scene;
    entity_t *winent;
    window_t *window;

    int colNdx;
    int rowNdx;
    vector_t direction;
    pixel_t testpixel = {255, 255, 255};
    pixel_t *pixelPtr;
    entity_t *testobj1, *testobj2;
    iterator_t *itr;

    scene = newScene();
    if (scene == NULL) {
       fprintf(stderr, "scene object not created, exiting\n");
       exit(1);
    }
    assert(scene->magic == SCENE_T);

    winent = newWindow("window", WINDOW);
    window = winent->entDerived;
    assert(window->magic == WINDOW_T);

    assert(window->magic == WINDOW_T);

    fprintf(stdout, "Enter viewpoint coordinates: ");
    if (fscanf(stdin, "%lf %lf %lf", &window->viewPoint.x, 
                                     &window->viewPoint.y,
                                     &window->viewPoint.z) != 3) {
       fprintf(stderr, "ERROR: could not input windowpoint coordinates\n");
       exit(1);
    }

    fprintf(stdout, "Enter window width and height: ");
    if (fscanf(stdin, "%lf %lf", &window->windowWidth, 
                                 &window->windowHeight) != 2) {
       fprintf(stderr, "ERROR: could not input scene width or height\n");
       exit(1);
    }

    fprintf(stdout, "Enter pixel width (columns): ");
    if (fscanf(stdin, "%d", &window->pixelColumns) != 1) {
       fprintf(stderr, "ERROR: could not input pixel width\n");
       exit(1);
    }

    completeWindow(scene, winent);
    assert(((window_t *)(scene->window->entDerived))->magic == WINDOW_T);

    completeScene(scene);

    /* scene dump */
    dumpScene(stdout, scene);


    /* Test to make sure image pixel area was allocated, pixelPtr
       is used in loop below.  What out for a seg fault in loop. 
    */
    assert(scene->picture->magic == IMAGE_T);
    pixelPtr = scene->picture->image;

    /* genRay test */
    fprintf(stdout, "\ngenRay test:\n");
    fprintf(stdout, " col  row    x       y       z\n");
    fflush(stdout);
    /* Generate a unit vector from the view point to each pixel */
    for (rowNdx=0; rowNdx<scene->picture->rows; rowNdx++) {
        for (colNdx=0; colNdx<scene->picture->columns; colNdx++) {
            direction = genRay(scene, colNdx, rowNdx);
            fprintf(stdout, "%4d %4d %7.4lf %7.4lf %7.4lf\n", 
                            colNdx,
                            rowNdx,
                            direction.x,
                            direction.y,
                            direction.z);

            /* Test to see if image pixel area was malloc'd and that the
               image field of the window_t is pointing to it.  This will
               seg fault if not.
            */
            *pixelPtr = testpixel;
            pixelPtr++;
        }
    }

    /* Make sure the lists in the scene_t have been 
       created 
    */
    fprintf(stdout, "\nObject lists test:\n"); fflush(stdout);
    testobj1 = newEntity("testing1", 99);
    testobj2 = newEntity("testing2", 99);
    l_add(scene->sobjList, testobj1);
    l_add(scene->sobjList, testobj2);
    l_add(scene->lightList, testobj1);
    l_add(scene->lightList, testobj2);

    itr = newIterator(scene->sobjList);
    testobj1 = l_next(itr);
    dumpEntity(stdout, testobj1);
    testobj1 = l_next(itr);
    dumpEntity(stdout, testobj1);
    printf("\n");

    /* And test with a bad pointer */
    testobj1 = (entity_t *)scene;
    dumpWindow(stdout, testobj1);

    exit(0);
}
