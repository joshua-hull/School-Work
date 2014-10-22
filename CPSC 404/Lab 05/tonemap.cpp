/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 5
 *
 * Tone map an HDR image using point and region processing.
 *
 * usage: tonemap -g value input_image [output_image]
 * 				tonemap -c input_image [output_image]
 */
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pixel.h"
#include <OpenImageIO/imageio.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

OIIO_NAMESPACE_USING

bool cOption;
bool gOption;
float gValue;

int width;
int height;
int channels;

static std::vector<float> oiioPixels;		// OpeImageIO copy of the pixels
static std::vector<float> openGLPixels;	// OpenGL copy of the pixels
rgba_pixel** pixels;     					      // The actual pixels of the image

char *outImage;								// File name for the output image
bool canWrite;								// Is there an ouput image name present
char *inImage;								// Input image name used for reset

/**
 * @brief Read in images
 * @details Read foregournd and background images into program.
 *
 * @param foreground Foreground image file path.
 * @param background Background image file path.
 */
void readImage(char *image) {

  // Open image input
  ImageInput *in = ImageInput::open(image);

  // Error handeling
  if (!in) {
    printf("Error reading image: %s\n", geterror().c_str());
    exit(EXIT_FAILURE);
  }

  // Get input spec
  const ImageSpec &spec = in->spec();

  // Get image details
  width = spec.width;
  height = spec.height;
  channels = spec.nchannels;

  // Init the global copy of the pixels
  // Read in the pixels and close the file
  oiioPixels.resize(width*height*channels*sizeof(float));
  in->read_image(TypeDesc::FLOAT, &oiioPixels[0]);
  in->close();
  delete in;

  // Initalize 2d array
  pixels = new rgba_pixel*[height];
  pixels[0] = new rgba_pixel[width*height];

  for (int i=1; i<height; i++) {
    pixels[i] = pixels[i-1] + width;
  }

  // Transfer into custom data structure
  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      pixels[row][col].r = oiioPixels[(row*width+col)*channels + 0];
      pixels[row][col].g = oiioPixels[(row*width+col)*channels + 1];
      pixels[row][col].b = oiioPixels[(row*width+col)*channels + 2];
      if(channels == 4) pixels[row][col].a = oiioPixels[(row*width+col)*channels + 3];
      else pixels[row][col].a = 1.0;
    }
}

/**
 * @brief Flip OpenGL.
 * @details Flip pixels for transfering to/from OpenGL.
 */
void openGLFlip() {
    openGLPixels.resize(width*height*4*sizeof(float));
    for (int row = 0; row < height; row++)
      for (int col = 0; col < width; col++){
        openGLPixels[((height - 1 - row)*width+col)*4 + 0] = pixels[row][col].r;
        openGLPixels[((height - 1 - row)*width+col)*4 + 1] = pixels[row][col].g;
        openGLPixels[((height - 1 - row)*width+col)*4 + 2] = pixels[row][col].b;
        openGLPixels[((height - 1 - row)*width+col)*4 + 3] = pixels[row][col].a;
    }
}

/**
 * @brief Draw composited image.
 * @details Draw composited image using OpenGL.
 */
void drawImage() {
  //Reset position to origin
    glRasterPos2i(0, 0);

    // Dump the pixels
    glDrawPixels(width, height, GL_RGBA, GL_FLOAT, &openGLPixels[0]);

    // Flush to screen.
    glFlush();
}

/**
 * Handle key presses. OpenGL keyboard callback.
 * @param key key code pressed
 * @param x   keyboard pos.
 * @param y   keyboard pos.
 */
void handleKey(unsigned char key, int x, int y) {
    switch (key) {
    //Gracefully exit the program
        case 'q':
        case 'Q':
        case 27:                    // ESC
            exit(EXIT_SUCCESS);

        default:
            return;
    }
}

void openGLSetup(int width, int height) {
  // Window setup
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  glutCreateWindow("filt - Joshua Hull (jhull@clemson.edu)");

  // Callback setup
  glutDisplayFunc(drawImage);
  glutKeyboardFunc(handleKey);

  // More window setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);

  glClearColor(1, 1, 1, 1);

}

int main(int argc, char** argv){
  cOption = false;
  gOption = false;
  gValue = 0.0;

  // Parse input parameters.
  char c;
  while ((c = getopt (argc, argv, "cg:")) != -1)
    switch (c) {
      case 'c':
        cOption = true;
        break;
      case 'g':
        gOption = true;
        gValue = atof(optarg);
        break;
      case '?':
        if (optopt == 'g')
          printf("-g requires a gamma value.\n");
          return 1;
      default:
        break;
    }

    if(!(cOption^gOption)) {
      printf("Only -g gamma or -c can be specified, not both or neither.\n");
      return 1;
    }

    if(cOption){
      if(argc == 4){
        canWrite = true;
        outImage = argv[3];
      } else {
        canWrite = false;
      }
      inImage = argv[2];
    } else {
      if(argc == 5) {
        canWrite = true;
        outImage = argv[4];
      } else {
        canWrite = false;
      }
      inImage = argv[3];
    }

    readImage(inImage);

    // Flip for openGL
    openGLFlip();

    // Init OpenGL
    glutInit(&argc, argv);
    openGLSetup(width, height);

    // Start running display window
    glutMainLoop();


  return 0;
}
