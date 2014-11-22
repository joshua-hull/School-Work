/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 7
 *
 * Warp an image, correcting for reconstruction artifacts.
 *
 * usage: warp input_image [output_image]
 */

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pixel.h"
#include <OpenImageIO/imageio.h>
#include <array>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include "vecmat/Matrix.h"

OIIO_NAMESPACE_USING

int width;
int height;
int channels;
static std::vector<float> oiioPixels;       // oiio pixels to read in and out
static std::vector<float> openGLPixels;	    // Pixels being displayed by OpenGL
rgba_pixel** pixels;                        // Original image pixels
rgba_pixel** warppedPixels;                 // Warpped image pixels
char *outImage;								              // File name for the output image
bool canWrite;

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
        openGLPixels[((height - 1 - row)*width+col)*4 + 0] = warppedPixels[row][col].r;
        openGLPixels[((height - 1 - row)*width+col)*4 + 1] = warppedPixels[row][col].g;
        openGLPixels[((height - 1 - row)*width+col)*4 + 2] = warppedPixels[row][col].b;
        openGLPixels[((height - 1 - row)*width+col)*4 + 3] = warppedPixels[row][col].a;
      }
}

float u(int x, int y) {
  return width * (1.0 - sqrt(1.0 - (x / (float)width))) * (1.6 + 0.5 * cos((2 * PI * y) / (float)height));
}

float v(int x, int y) {
  return height * (1.0 - sqrt(1.0 - (y / (float)height))) * (1.6 + 0.5 * cos((2 * PI * x) / (float)width));
}

/**
 * Process the image by averaging the four nearest pixels.
 */
void smartProcess() {
  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      float U = round(u(col,row));
      float V = round(v(col,row));
      if(!((0 <= U && U < width ) && (0 <= V && V < height ))) {
        rgba_pixel p;
        p.r = 0.0;
        p.g = 0.0;
        p.b = 0.0;
        p.a = 1.0;
        warppedPixels[row][col] = p;
      }
      else {
        int U1, U2, U3, U4;
        int V1, V2, V3, V4;
        rgba_pixel p, p1, p2, p3, p4;

        U1 = U2 = floor(U);

        U3 = U4 = floor(U + 1);
        U3 = U4 = U3 % width;

        V1 = V4 = floor(V + 1);
        V1 = V4 = V4 % height;

        V2 = V3 = floor(V);

        p1 = pixels[V1][U1];
        p2 = pixels[V2][U2];
        p3 = pixels[V3][U3];
        p4 = pixels[V4][U4];

        std::array<rgba_pixel,4> neightborhood = {p1,p2,p3,p4};

        float redSum = 0.0;
        float greenSum = 0.0;
        float blueSum = 0.0;
        float alphaSum = 0.0;
        for ( auto it = neightborhood.begin(); it != neightborhood.end(); ++it ) {
          rgba_pixel p = *it;
          redSum += p.r;
          greenSum += p.g;
          blueSum += p.b;
          alphaSum += p.a;
        }
        redSum /= 4;
        greenSum /= 4;
        blueSum /= 4;
        alphaSum /= 4;

        p.r = redSum;
        p.g = greenSum;
        p.b = blueSum;
        p.a = alphaSum;

        warppedPixels[row][col] = p;
      }
  }
}

/**
 * Perform the warp using nearest neightbor interpolation.
 */
void dumbProcess() {

  // Initalize 2d array
  warppedPixels = new rgba_pixel*[height];
  warppedPixels[0] = new rgba_pixel[width*height];

  for (int i=1; i<height; i++) {
    warppedPixels[i] = warppedPixels[i-1] + width;
  }

  for (int row = 0; row < height; row++)
      for (int col = 0; col < width; col++){
        int U = (int)round(u(col,row));
        int V = (int)round(v(col,row));
        if((0 <= U && U < width ) && (0 <= V && V < height )) warppedPixels[row][col] = pixels[V][U];
        else {
          rgba_pixel p;
          p.r = 0.0;
          p.g = 0.0;
          p.b = 0.0;
          p.a = 1.0;
          warppedPixels[row][col] = p;
        }
     }

}

/**
* @brief Write imgage.
* @details Write compostied image to the file system.
*/
void writeImage() {

  // Transfer to something OpenImageIO understands
  oiioPixels.resize(width*height*4*sizeof(float));

  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      oiioPixels[(row*width+col)*4 + 0] = warppedPixels[row][col].r;
      oiioPixels[(row*width+col)*4 + 1] = warppedPixels[row][col].g;
      oiioPixels[(row*width+col)*4 + 2] = warppedPixels[row][col].b;
      oiioPixels[(row*width+col)*4 + 3] = warppedPixels[row][col].a;
    }

    // Create output image
    ImageOutput *out = ImageOutput::create(outImage);

    // Error handeling
    if (!out) {
      printf("Error writing image: %s\n", geterror().c_str());
      exit(EXIT_FAILURE);
    }

    // Create output image spec
    ImageSpec spec (width, height, 4, TypeDesc::FLOAT);

    // Open output image file
    out->open(outImage, spec);

    // Write output image to disk and close
    out->write_image(TypeDesc::FLOAT, &oiioPixels[0]);
    out->close();
    delete out;
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

        case 'r':
        case 'R':
          smartProcess();
          openGLFlip();
          glutPostRedisplay();
          if(canWrite) writeImage();
          break;

        default:
          return;
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
 * Setup the openGL enviorment.
 * @param width  Width of the window.
 * @param height Height of the window.
 */
void openGLSetup(int width, int height) {
  // Window setup
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  glutCreateWindow("warp - Joshua Hull (jhull@clemson.edu)");

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
 * Main program
 * @param  argc Number of command line arguments, inlucing the program itself.
 * @param  argv Vector of command line arguments.
 * @return      EXIT_SUCCESS if program exits normally, EXIT_ERROR otherwise.
 */
int main(int argc, char** argv) {
  if(argc < 2 || argc > 3) {
    std::cout << "Usage: warper input_image [ouput_image]" << std::endl;
    return EXIT_FAILURE;
  }

  readImage(argv[1]);

  dumbProcess();

  // Flip for openGL
  openGLFlip();

  // Init OpenGL
  glutInit(&argc, argv);
  openGLSetup(width, height);

  if(argc == 3) {
    outImage = argv[2];
    canWrite = true;
  }

  // Start running display window
  glutMainLoop();

  return EXIT_SUCCESS;
}
