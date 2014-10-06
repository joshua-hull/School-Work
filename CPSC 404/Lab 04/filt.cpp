/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 4
 *
 * Filter a given input image with a given filter using the convolution method.
 *
 * usage: filt filter_file input_image [output_image]
 */

#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <OpenImageIO/imageio.h>
#include <iostream>
#include "pixel.h"
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

OIIO_NAMESPACE_USING

// Global variables to keep things a little simpler
int width;			    					// Image width
int height;				            		// Image height
int channels;			            		// Number of channels in the image
rgba_pixel** pixels;     					// The actual pixels of the image
rgba_pixel** convolution;					// Convolution working copy of the pixels

char *outImage;								// File name for the output image
bool canWrite;								// Is there an ouput image name present

static std::vector<float> oiioPixels;		// OpeImageIO copy of the pixels
static std::vector<float> openGLPixels;		// OpenGL copy of the pixels

std::vector<std::vector <float> > filter;	// 2d vecrtor representing the filter

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
     if (!in)
     {
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
 * @brief Write imgage.
 * @details Write compostied image to the file system.
 */
void writeImage() {

	// Transfer to something OpenImageIO understands
     oiioPixels.resize(width*height*4*sizeof(float));

     for (int row = 0; row < height; row++)
         for (int col = 0; col < width; col++){
          oiioPixels[(row*width+col)*4 + 0] = pixels[row][col].r;
          oiioPixels[(row*width+col)*4 + 1] = pixels[row][col].g;
          oiioPixels[(row*width+col)*4 + 2] = pixels[row][col].b;
          oiioPixels[(row*width+col)*4 + 3] = pixels[row][col].a;
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

inline int modulo(int a, int b) {
  const int result = a % b;
  return result >= 0 ? result : result + b;
}

/**
 * @brief Convolute
 * @details Convolute inout image with filter
 */
void convolutionFilter() {
    convolution = new rgba_pixel*[height];
    convolution[0] = new rgba_pixel[width*height];

    for (int i=1; i<height; i++) {
       convolution[i] = convolution[i-1] + width;
    }

   for(int row = 0; row < height; row++)
     for(int col = 0; col < width; col++){
        rgba_pixel pixel;
        pixel.r = 0.0;
        pixel.g = 0.0;
        pixel.b = 0.0;
        pixel.a = 0.0;

        float negSum = 0.0;
        float posSum = 0.0;
        int radius = (int)filter[0].size()/2;

        // Calculate the convolution using the tiling method for the boundry.
        for(int i = -radius; i <= radius; i++)
           for(int j = -radius; j <= radius; j++){
              pixel.r += pixels[modulo(row + i,height)][modulo(col + j,width)].r*filter[i + radius][j + radius];
              pixel.g += pixels[modulo(row + i,height)][modulo(col + j,width)].g*filter[i + radius][j + radius];
              pixel.b += pixels[modulo(row + i,height)][modulo(col + j,width)].b*filter[i + radius][j + radius];
              pixel.a += pixels[modulo(row + i,height)][modulo(col + j,width)].a*filter[i + radius][j + radius];

              if(filter[i + radius][j + radius] > 0) posSum += filter[i + radius][j + radius];
              else negSum += abs(filter[i + radius][j + radius]);
          }
          pixel.r /= fmax(negSum,posSum);
          pixel.g /= fmax(negSum,posSum);
          pixel.b /= fmax(negSum,posSum);
          pixel.a /= fmax(negSum,posSum);

          convolution[row][col] = pixel;
      }

      // Copy the convolution over and redraw the image.
      for(int row = 0; row < height; row++)
          for(int col = 0; col < width; col++){
            pixels[row][col].r = convolution[row][col].r;
            pixels[row][col].b = convolution[row][col].b;
            pixels[row][col].g = convolution[row][col].g;
            pixels[row][col].a = convolution[row][col].a;
        }

    openGLFlip();
    glutPostRedisplay();
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

        //Write the image file out
        case 'w':
        case 'W':
            if (canWrite) writeImage();
            return;

        //Readload original image
        case 'r':
        case 'R':
            readImage(inImage);
            openGLFlip();
            glutPostRedisplay();
            return;

        //Convolve the image
        case 'c':
        case 'C':
            convolutionFilter();
            return;

        default:
            return;
    }
}

/**
 * Setup OpenGL window.
 * @param width  Window width
 * @param height Window height
 */
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

/**
 * @brief Read filter
 * @details Read filter in from file    
 * 
 * @param file Inout file name
 */
void readFilter(char* file) {
	// Open the filter file
	std::ifstream input(file);

	// Error checking
	if (input == NULL) {
		printf("Error reading filter");
		exit(1);
	}
	// Get the size of the filter
	int count;
	input >> count;

	// Init 2d vector for the filter
	std::vector<float> row(count,0);
	filter = std::vector<std::vector<float> >(count,row);

	// Read in the contents of the filter
	for(int i = 0; i < count; i++)
		for(int j = 0; j < count; j++)
			input >> filter[i][j];

	// Close the filter file
    input.close();
}

int main(int argc, char** argv){
    if (argc != 3 && argc != 4){
        printf("Usage: %s filter_file input_image [output_image]\n", argv[0]);
        return 1;
    }

    inImage = argv[2];

    if(argc == 4) {
        outImage = argv[3];
        canWrite = true;
    }

    readImage(argv[2]);
    readFilter(argv[1]);

    // Flip for openGL
    openGLFlip();

    // Init OpenGL
    glutInit(&argc, argv);
    openGLSetup(width, height);

    // Start running display window
    glutMainLoop();

    return 0;
}