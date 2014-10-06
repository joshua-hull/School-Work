#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <OpenImageIO/imageio.h>
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
void readImage(char *image){

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
void writeImage(){

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
    if (!out)
    {
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

inline int modulo(int a, int b) {
  	const int result = a % b;
 	return result >= 0 ? result : result + b;
}

void convolution(){

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

   			float sum = 0.0;
   			int radius = (int)filter[0].size()/2;

   			for(int i = -radius; i <= radius; i++)
   				for(int j = -radius; j <= radius; j++){
   					pixel.r += pixels[row + i][col + j].r*filter[i + radius][j + radius];
   					pixel.g += pixels[row + i][col + j].g*filter[i + radius][j + radius];
   					pixel.b += pixels[row + i][col + j].b*filter[i + radius][j + radius];
   					pixel.a += pixels[row + i][col + j].a*filter[i + radius][j + radius];

   					sum += filter[i + radius][j + radius];
   				}
   			pixel.r /= sum;
   			pixel.g /= sum;
   			pixel.b /= sum;
   			pixel.a /= sum;

   			convolution[row][col] = pixel;
   		}
}

/**
 * @brief Draw composited image.
 * @details Draw composited image using OpenGL.
 */
void drawImage(){
	//Reset position to origin
    glRasterPos2i(0, 0);

    // Dump the pixels
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, &openGLPixels[0]);

    // Flush to screen.
    glFlush();
}

/**
 * @brief Flip OpenGL.
 * @details Flip pixels for transfering to/from OpenGL.
 */
void openGLFlip(){
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
 * Handle key presses. OpenGL keyboard callback.
 * @param key key code pressed
 * @param x   keyboard pos.
 * @param y   keyboard pos.
 */
void handleKey(unsigned char key, int x, int y)
{
    switch (key)
    {
    //Gracefully exit the program
    case 'q':
    case 'Q':
    case 27:                    // ESC
        exit(EXIT_SUCCESS);

    //Write the image file out
    case 'w':
    case 'W':
        if (canWrite) writeImage();

    //Readload original image
    case 'r':
    case 'R':
    	readImage(inImage);
    	openGLFlip();

    //Convolve the image
    case 'c':
    case 'C':
    	convolution();
    default:
        return;
    }
}

/**
 * Setup OpenGL window.
 * @param width  Window width
 * @param height Window height
 */
void openGLSetup(int width, int height)
{
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