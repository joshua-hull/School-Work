#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING

void writeImage() {
}

void drawImage() {
        glFlush();
}

void handleKey(unsigned char key, int x, int y) {
          
    switch(key){
        //Gracefully exit the program.
        case 'q':       
        case 'Q':
        case 27:  
            exit(EXIT_SUCCESS);
                  
        //Write the image file out.
        case 'w':
        case 'W':
            writeImage();        
        default:
            return;
                                                              }
}

void openGLSetup(int width, int height) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("oiioview - Joshua Hull (jhull@clemson.edu)");

    glutDisplayFunc(drawImage);
    glutKeyboardFunc(handleKey);
        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
               
    glClearColor(1, 1, 1, 1);
               
}

void readImage(char* inImage) {

    ImageInput *in = ImageInput::open (inImage);
    if (! in)
            return;
    const ImageSpec &spec = in->spec();
    int xres = spec.width;
    int yres = spec.height;
    int channels = spec.nchannels;
    std::vector<unsigned char> pixels (xres*yres*channels);
    in->read_image (TypeDesc::UINT8, &pixels[0]);
    in->close ();
    delete in;

    printf("%d %d\n", xres, yres);
}

int main (int argc, char** argv) {
    glutInit(&argc, argv);
    openGLSetup(100, 100);
    readImage(argv[1]);
    glutMainLoop();
    return EXIT_SUCCESS;
}
