/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 6
 *
 * Warp an image given the specified parameters.
 *
 * usage: warper input_image [output_image]
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
#include "vecmat/Matrix.h"
#include "process.h"

OIIO_NAMESPACE_USING

int originalWidth;
int originalHeight;
int channels;
static std::vector<float> oiioPixels;
static std::vector<float> openGLPixels;	   // Pixels being displayed by OpenGL
rgba_pixel** pixels;
rgba_pixel** warppedPixels;

int newWidth;
int newHeight;

/**
 * @brief Read in images
 * @details Read foregournd and background images into program.
 *
 * @param image Image name to read in.
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
  originalWidth = spec.width;
  originalHeight = spec.height;
  channels = spec.nchannels;

  // Init the global copy of the pixels
  // Read in the pixels and close the file
  oiioPixels.resize(originalWidth*originalHeight*channels*sizeof(float));
  in->read_image(TypeDesc::FLOAT, &oiioPixels[0]);
  in->close();
  delete in;

  // Initalize 2d array
  pixels = new rgba_pixel*[originalHeight];
  pixels[0] = new rgba_pixel[originalWidth*originalHeight];

  for (int i=1; i<originalHeight; i++) {
    pixels[i] = pixels[i-1] + originalWidth;
  }

  // Transfer into custom data structure
  for (int row = 0; row < originalHeight; row++)
    for (int col = 0; col < originalWidth; col++){
      pixels[row][col].r = oiioPixels[(row*originalWidth+col)*channels + 0];
      pixels[row][col].g = oiioPixels[(row*originalWidth+col)*channels + 1];
      pixels[row][col].b = oiioPixels[(row*originalWidth+col)*channels + 2];
      if(channels == 4) pixels[row][col].a = oiioPixels[(row*originalWidth+col)*channels + 3];
      else pixels[row][col].a = 1.0;
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
    glDrawPixels(newWidth, newHeight, GL_RGBA, GL_FLOAT, &openGLPixels[0]);

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
          break;

        default:
          return;
    }
}


/**
 * Setup the openGL enviorment.
 * @param width  Width of the window.
 * @param height Height of the window.
 */
void openGLSetup(int width, int height) {
  // Window setup
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  glutCreateWindow("tonemap - Joshua Hull (jhull@clemson.edu)");

  // Callback setup
  glutDisplayFunc(drawImage);
  glutKeyboardFunc(handleKey);

  // More window setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);

  glClearColor(1, 1, 1, 1);

}

/**
 * @brief Flip OpenGL.
 * @details Flip pixels for transfering to/from OpenGL.
 */
void openGLFlip() {
    openGLPixels.resize(newWidth*newHeight*4*sizeof(float));
    for (int row = 0; row < newHeight; row++)
      for (int col = 0; col < newWidth; col++){
        std::cout << col << "," << row << std::endl;
        openGLPixels[((newHeight - 1 - row)*newWidth+col)*4 + 0] = pixels[row][col].r;
        openGLPixels[((newHeight - 1 - row)*newWidth+col)*4 + 1] = pixels[row][col].g;
        openGLPixels[((newHeight - 1 - row)*newWidth+col)*4 + 2] = pixels[row][col].b;
        openGLPixels[((newHeight - 1 - row)*newWidth+col)*4 + 3] = pixels[row][col].a;
      }
}

/**
 * Main program
 * @param  argc Number of command line arguments, inlucing the porgram itself.
 * @param  argv Vector of command line arguments.
 * @return      EXIT_SUCCESS if program exits normally, EXIT_ERROR otherwise.
 */
int main(int argc, char** argv) {
  if(argc < 2 || argc > 3) {
    std::cout << "Usage: warper input_image [ouput_image]" << std::endl;
    return EXIT_FAILURE;
  }

  readImage(argv[1]);

  Matrix3x3 M(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);
  process_input(M, originalWidth, originalHeight);

  Vector3d upperRight(originalWidth-1,originalHeight-1, 1);
  Vector3d lowerRight(originalWidth-1,0, 1);
  Vector3d upperLeft(0,originalHeight-1, 1);
  Vector3d lowerLeft(0,0, 1);

  upperRight = (upperRight * M)/upperRight[2];
  lowerRight = (lowerRight * M)/lowerRight[2];
  upperLeft = (upperLeft * M)/upperLeft[2];
  lowerLeft = (lowerLeft * M)/lowerLeft[2];

  std::cout << upperLeft << lowerLeft << upperRight << lowerRight << std::endl;

  newWidth = max(abs(upperRight[1] - lowerRight[1]), abs(upperLeft[1] - lowerLeft[1]));
  newHeight = max(abs(lowerRight[0] - lowerLeft[0]), abs(upperRight[0] - upperLeft[0]));

  int originX = min(lowerLeft[0], lowerRight[0]);
  int originY = min(lowerLeft[1], upperLeft[1]);

  Vector3d newOrigin(originX, originY, 1);

  // Initalize 2d array
  warppedPixels = new rgba_pixel*[newHeight];
  warppedPixels[0] = new rgba_pixel[newWidth*newHeight];

  for (int i=1; i<originalHeight; i++) {
    warppedPixels[i] = warppedPixels[i-1] + newWidth;
  }

  Matrix3x3 invM = M.inv();

  std::cout << "New Width: " << newWidth << " New Height: " << newHeight << std::endl;

  for(int row = 0; row < newHeight; row++)
    for(int col = 0; col < newWidth; col++) {
      //map the pixel coordinates
      Vector3d pixel_out(col, row, 1);
      pixel_out = pixel_out + newOrigin;
      Vector3d pixel_in = invM * pixel_out;
      //normalize the pixmap
      float u = pixel_in[0] / pixel_in[2];
      float v = pixel_in[1] / pixel_in[2];
      //Could use better interpolation
      warppedPixels[(int)round(u)][(int)round(v)] = pixels[col][row];
    }

    // Flip for openGL
    std::cout << "Start flip..." << std::endl;
    openGLFlip();
    std::cout << "End flip..." << std::endl;

    // Init OpenGL
    glutInit(&argc, argv);

    std::cout << "Start setup..." << std::endl;
    openGLSetup(newWidth, newHeight);
    std::cout << "End setup..." << std::endl;

    // Start running display window
    glutMainLoop();

  return EXIT_SUCCESS;
}
