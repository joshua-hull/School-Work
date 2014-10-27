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

static std::vector<float> oiioPixels;		   // OpeImageIO copy of the pixels
static std::vector<float> openGLPixels;	   // Pixels being displayed by OpenGL
rgba_pixel** toneMappedPixels;                 // Tone mapped image data.
rgba_pixel** pixels;     					         // The actual pixels of the image

char *outImage;								// File name for the output image
bool canWrite;								// Is there an ouput image name present
char *inImage;								// Input image name used for reset

bool toneMapped;              // Are we displaying the tone mapped image?

std::vector<std::vector <float> > filter;	// 2d vecrtor representing the filter
int count = 19;

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
        if(!toneMapped){
          openGLPixels[((height - 1 - row)*width+col)*4 + 0] = pixels[row][col].r;
          openGLPixels[((height - 1 - row)*width+col)*4 + 1] = pixels[row][col].g;
          openGLPixels[((height - 1 - row)*width+col)*4 + 2] = pixels[row][col].b;
          openGLPixels[((height - 1 - row)*width+col)*4 + 3] = pixels[row][col].a;
        } else {
          openGLPixels[((height - 1 - row)*width+col)*4 + 0] = toneMappedPixels[row][col].r;
          openGLPixels[((height - 1 - row)*width+col)*4 + 1] = toneMappedPixels[row][col].g;
          openGLPixels[((height - 1 - row)*width+col)*4 + 2] = toneMappedPixels[row][col].b;
          openGLPixels[((height - 1 - row)*width+col)*4 + 3] = toneMappedPixels[row][col].a;
        }
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
 * @brief Write imgage.
 * @details Write compostied image to the file system.
 */
void writeImage() {

  // Transfer to something OpenImageIO understands
     oiioPixels.resize(width*height*4*sizeof(float));

     for (int row = 0; row < height; row++)
         for (int col = 0; col < width; col++){
          oiioPixels[(row*width+col)*4 + 0] = toneMappedPixels[row][col].r;
          oiioPixels[(row*width+col)*4 + 1] = toneMappedPixels[row][col].g;
          oiioPixels[(row*width+col)*4 + 2] = toneMappedPixels[row][col].b;
          oiioPixels[(row*width+col)*4 + 3] = toneMappedPixels[row][col].a;
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

        case 's':
        case 'S':
          toneMapped = !toneMapped;
          openGLFlip();
          glutPostRedisplay();
          break;

        case 'w':
        case 'W':
          if(canWrite) writeImage();
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
 * Perform a mathmatically correct modulos.
 * @param  a Number to mod.
 * @param  b Number modded by.
 * @return   a mod b
 */
inline int modulo(int a, int b) {
  const int result = a % b;
  return result >= 0 ? result : result + b;
}

/**
 * Convolve an array of floats with a given filter.
 * @param  original The original array of floats.
 * @param  filter   The filter to convolve with.
 * @return          original convolved with filter.
 */
float** convolution(float** original, std::vector<std::vector <float> > filter){
  float** convolution = new float*[height];

  for(int row = 0; row < height; row++){
    convolution[row] = new float[width];
    for(int col = 0; col < width; col++){
      float tmp = 0.0f;

      float negSum = 0.0;
      float posSum = 0.0;
      int radius = (int)filter[0].size()/2;

      // Calculate the convolution using the tiling method for the boundry.
      for(int i = -radius; i <= radius; i++)
         for(int j = -radius; j <= radius; j++){
            tmp += pixels[modulo(row + i,height)][modulo(col + j,width)].r*filter[i + radius][j + radius];

            if(filter[i + radius][j + radius] > 0) posSum += filter[i + radius][j + radius];
            else negSum += abs(filter[i + radius][j + radius]);
        }
        tmp /= fmax(negSum,posSum);

        convolution[row][col] = tmp;
    }
  }
  return convolution;
}

/**
 * Tone map the pixels in the image.
 */
void toneMapImage(){

  // Initalize 2d array
  toneMappedPixels = new rgba_pixel*[height];
  toneMappedPixels[0] = new rgba_pixel[width*height];

  for (int i=1; i<height; i++) {
    toneMappedPixels[i] = toneMappedPixels[i-1] + width;
  }

  // Simple Tone Mapping
  if(gOption) {
    for(int row = 0; row < height; row++)
      for(int col = 0; col < width; col++){
        rgba_pixel p = pixels[row][col];

        // L_w = (20.0R+40.0G+B)/61.0
        float l_w = (20*p.r + 40*p.g + p.b)/61.0;
        float tmp = log(l_w);

        // log(L_d) = gamma * log(L_w)
        float tmp2 = gValue * tmp;
        float l_d = exp(tmp2);

        // C_d = (L_d/L_w) * C
        rgba_pixel tmpP = p * (l_d/l_w);

        toneMappedPixels[row][col] = tmpP;
      }
  }
  // Tone Mapping with Convolution
  else {
      float** B = new float*[height];
      float** S = new float*[height];
      float** l_w = new float*[height];
      float** l_d = new float*[height];
      float gamma;
      float min_b = 1.0f;
      float max_b = 0.0f;
      int c = 100;

      for (int i=0; i<height; i++) {
        B[i] = new float[width];
        S[i] = new float[width];
        l_w[i] = new float[width];
        l_d[i] = new float[width];
      }

      // log(L_w)
      for(int row = 0; row < height; row++)
        for(int col = 0; col < width; col++){
          rgba_pixel p = pixels[row][col];
          l_w[row][col] = log((20*p.r + 40*p.g + p.b)/61.0);
        }

      // B = log(L_w) (x) g
      B = convolution(l_w, filter);

      // S = log(L_w) - B
      // Also calculate max(B) and min(B)
      for(int row = 0; row < height; row++)
        for(int col = 0; col < width; col++){
          S[row][col] = l_w[row][col] - B[row][col];
          min_b = fmin(min_b, B[row][col]);
          max_b = fmax(max_b, B[row][col]);
        }

      // gamma = log(c)/(max(B)-min(B))
      gamma = log(c)/float(max_b-min_b);

      // log(L_d) = gamma * B + S
      for(int row = 0; row < height; row++)
        for(int col = 0; col < width; col++){
          l_d[row][col] = gamma * B[row][col] + S[row][col];
        }

      // L_d = exp(log(L_d))
      // L_w = exp(log(L_w))
      // C_d = (L_d/L_w)*C
      for(int row = 0; row < height; row++)
        for(int col = 0; col < width; col++){
          toneMappedPixels[row][col] = exp(l_d[row][col])/exp(l_w[row][col]) * pixels[row][col];
        }
  }
}

int main(int argc, char** argv){
  cOption = false;  // Are we performing a convolution tone mapping?
  gOption = false;  // Are we performing a simple tone mapping?
  gValue = 0.0;     // Gamma value for simple tone mapping.

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
          return EXIT_FAILURE;
      default:
        break;
    }

    // Error checking.
    if(!(cOption^gOption)) {
      printf("Only -g gamma or -c can be specified, not both or neither.\n");
      return EXIT_FAILURE;
    }

    // Parse the rest of the parameters.
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

    // Init 2d vector for the filter
    std::vector<float> row(count,0);
    filter = std::vector<std::vector<float> >(count,row);

    // Read in the contents of the filter
    for(int i = count-1; i >= 0; i--)
      for(int j = count - 1; j >= 0; j--)
        filter[i][j] = 1.0;

    readImage(inImage);
    toneMapImage();

    // Flip for openGL
    openGLFlip();

    // Init OpenGL
    glutInit(&argc, argv);
    openGLSetup(width, height);

    // Start running display window
    glutMainLoop();

  return EXIT_SUCCESS;
}
