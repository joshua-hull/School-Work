/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 3
 *
 * Calcualte the alpha mask of a greenscreen image.
 *
 * usage: alphamask input_image output_image
 */
#include <cstdlib>
#include <stdio.h>
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
int width_foreground;                              	// Foregound image width
int height_foreground;                            	// Foregound image height
int channels_foreground;                           	// Number of channels in the foreground image
rgba_pixel** pixels_foreground;      				// The actual pixels of the foreground image

int width_background;                              	// Background image width
int height_background;                            	// Background image height
int channels_background;                           	// Number of channels in the background image
rgba_pixel** pixels_background;      				// The actual pixels of the background image

static std::vector<float> oiioPixels;				// OpeImageIO copy of the pixels
static std::vector<float> openGLPixels;				// OpenGL copy of the pixels
rgba_pixel** pixels_composited;						// Compositied pixels

char *outImage;
bool canWrite;

/**
 * @brief Read in images
 * @details Read foregournd and background images into program.
 * 
 * @param foreground Foreground image file path.
 * @param background Background image file path.
 */
void readImage(char *foreground, char* background){

	// Foreground Image
	// Open image input
	ImageInput *in = ImageInput::open(foreground);

	// Error handeling
	if (!in)
	{
	    printf("Error reading foreground image: %s\n", geterror().c_str());
	    exit(EXIT_FAILURE);
	}

	// Get input spec
	const ImageSpec &spec = in->spec();

	// Get image details
	width_foreground = spec.width;
	height_foreground = spec.height;
	channels_foreground = spec.nchannels;

	if (channels_foreground != 4){
		printf("ERROR: Foreground image must have an alpha channel.\n");
		exit(1);
	}

	// Init the global copy of the pixels
	// Read in the pixels and close the file
	oiioPixels.resize(width_foreground*height_foreground*channels_foreground*sizeof(float));
	in->read_image(TypeDesc::FLOAT, &oiioPixels[0]);
	in->close();
	delete in;

	// Initalize 2d array
	pixels_foreground = new rgba_pixel*[height_foreground];
   	pixels_foreground[0] = new rgba_pixel[width_foreground*height_foreground];

   	for (int i=1; i<height_foreground; i++) {
    	pixels_foreground[i] = pixels_foreground[i-1] + width_foreground;
   	}

	// Transfer into custom data structure
	for (int row = 0; row < height_foreground; row++)
	    for (int col = 0; col < width_foreground; col++){
	    	pixels_foreground[row][col].r = oiioPixels[(row*width_foreground+col)*channels_foreground + 0];
	    	pixels_foreground[row][col].g = oiioPixels[(row*width_foreground+col)*channels_foreground + 1];
	    	pixels_foreground[row][col].b = oiioPixels[(row*width_foreground+col)*channels_foreground + 2];
	    	pixels_foreground[row][col].a = oiioPixels[(row*width_foreground+col)*channels_foreground + 3];
	    }

	// Background Image
	// Open image input
	in = ImageInput::open(background);

	// Error handeling
	if (!in)
	{
	    printf("Error reading background image: %s\n", geterror().c_str());
	    exit(EXIT_FAILURE);
	}

	// Get input spec
	const ImageSpec &spec2 = in->spec();

	// Get image details
	width_background= spec2.width;
	height_background = spec2.height;
	channels_background = spec2.nchannels;

	// Init the global copy of the pixels
	// Read in the pixels and close the file
	oiioPixels.resize(width_background*height_background*channels_background*sizeof(float));
	in->read_image(TypeDesc::FLOAT, &oiioPixels[0]);
	in->close();
	delete in;

	// Initalize 2d array
	pixels_background = new rgba_pixel*[height_background];
   	pixels_background[0] = new rgba_pixel[width_background*height_background];

   	for (int i=1; i<height_background; i++) {
    	pixels_background[i] = pixels_background[i-1] + width_background;
   	}

	// Transfer into custom data structure
	for (int row = 0; row < height_background; row++)
	    for (int col = 0; col < width_background; col++){
	    	pixels_background[row][col].r = oiioPixels[(row*width_background+col)*channels_background + 0];
	    	pixels_background[row][col].g = oiioPixels[(row*width_background+col)*channels_background + 1];
	    	pixels_background[row][col].b = oiioPixels[(row*width_background+col)*channels_background + 2];
	    	// Handle the fact that the background may not have an alpha channel
	    	if (channels_background == 4){
	    		pixels_background[row][col].a = oiioPixels[(row*width_background+col)*channels_background + 3];
	    	} else{
	    		pixels_background[row][col].a = 1.0;
	    	}
	    }
}

/**
 * @brief Calculate composite.
 * @details Calcualte the composite A Over B.
 * 
 * @param A Foreground pixel.
 * @param B Background pixel.
 * 
 * @return Composited pixel.
 */
rgba_pixel AOverB(rgba_pixel A, rgba_pixel B){
	rgba_pixel retVal;

	// Compute and clamp each of {r,g,b,a} for A Over B
	float tmp = A.r*A.a + (B.r*B.a) * (1-A.a);
	retVal.r = std::max(0.0f, std::min(tmp, 1.0f));

	tmp = A.g*A.a + (B.g*B.a) * (1-A.a);
	retVal.b = std::max(0.0f, std::min(tmp, 1.0f));

	tmp = A.b*A.a + (B.b*B.a) * (1-A.a);
	retVal.g = std::max(0.0f, std::min(tmp, 1.0f));

	tmp = A.a*A.a + (B.a*B.a) * (1-A.a);
	retVal.a = std::max(0.0f, std::min(tmp, 1.0f));

	return retVal;
}

/**
 * @brief Compostite images.
 * @details Composite the images together.
 */
void compositeImages(){
	// Initalize 2d array
	pixels_composited = new rgba_pixel*[height_background];
   	pixels_composited[0] = new rgba_pixel[width_background*height_background];

   	for (int i=1; i<height_background; i++) {
    	pixels_composited[i] = pixels_composited[i-1] + width_background;
   	}
	for (int row = 0; row < height_background; row++)
	    for (int col = 0; col < width_background; col++){
	    	pixels_composited[row][col] = AOverB(pixels_foreground[row][col], pixels_background[row][col]);
	    }
}

/**
 * @brief Write imgage.
 * @details Write compostied image to the file system.
 */
void writeImage(){

	// Transfer to something OpenImageIO understands
	oiioPixels.resize(width_background*height_background*4*sizeof(float));

	for (int row = 0; row < height_background; row++)
	    for (int col = 0; col < width_background; col++){
	    	oiioPixels[(row*width_background+col)*4 + 0] = pixels_composited[row][col].r;
	    	oiioPixels[(row*width_background+col)*4 + 1] = pixels_composited[row][col].g;
	    	oiioPixels[(row*width_background+col)*4 + 2] = pixels_composited[row][col].b;
	    	oiioPixels[(row*width_background+col)*4 + 3] = pixels_composited[row][col].a;
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
    ImageSpec spec (width_background, height_background, 4, TypeDesc::FLOAT);

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
    default:
        return;
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
	glDrawPixels(width_background, height_background, GL_RGBA, GL_FLOAT, &openGLPixels[0]);

    // Flush to screen.
    glFlush();
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
    glutCreateWindow("composite - Joshua Hull (jhull@clemson.edu)");

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
void openGLFlip(){
	openGLPixels.resize(width_background*height_background*4*sizeof(float));
	for (int row = 0; row < height_background; row++)
	    for (int col = 0; col < width_background; col++){
	    	openGLPixels[((height_background - 1 - row)*width_foreground+col)*channels_foreground + 0] = pixels_composited[row][col].r;
	    	openGLPixels[((height_background - 1 - row)*width_foreground+col)*channels_foreground + 1] = pixels_composited[row][col].g;
	    	openGLPixels[((height_background - 1 - row)*width_foreground+col)*channels_foreground + 2] = pixels_composited[row][col].b;
	    	openGLPixels[((height_background - 1 - row)*width_foreground+col)*channels_foreground + 3] = pixels_composited[row][col].a;
	    }
}

int main(int argc, char** argv){
	if (argc != 3 && argc != 4){
        	printf("Usage: %s foreground_image background_image [output_image]\n", argv[0]);
       		return 1;
    }

    if(argc == 4) {
    	outImage = argv[3];
    	canWrite = true;
	}

	readImage(argv[1], argv[2]);
	compositeImages();

	// Flip for openGL
	openGLFlip();

	// Init OpenGL
    glutInit(&argc, argv);
    openGLSetup(width_background, height_background);

    // Start running display window
    glutMainLoop();

	return(0);
}