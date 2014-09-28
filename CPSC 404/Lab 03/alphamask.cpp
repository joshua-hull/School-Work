#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <OpenImageIO/imageio.h>
#include "pixel.h"

OIIO_NAMESPACE_USING

// Global variables to keep things a little simpler
int width;                              // Image width
int height;                             // Image height
int channels;                           // Number of channels in the image
rgba_pixel** pixels;      				// The actual pixels of the image
static std::vector<float> oiioPixels;		// OpeImageIO copy of the pixels

void readImage(char *inputPath){

	// Open image input
	ImageInput *in = ImageInput::open(inputPath);

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
	oiioPixels.resize(width*height*channels);
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
	    	pixels[row][col].a = 1.0; // We'll change this in processImage()
	    }
}

void processImage(){

}

void writeImage(char *outImage){

	// Transfer to something OpenImageIO understands
	oiioPixels.resize(width*height*4);

	for (int row = 0; row < height; row++)
	    for (int col = 0; col < width; col++){
	    	oiioPixels[(row*width+col)*channels + 0] = pixels[row][col].r;
	    	oiioPixels[(row*width+col)*channels + 1] = pixels[row][col].g;
	    	oiioPixels[(row*width+col)*channels + 2] = pixels[row][col].b;
	    	oiioPixels[(row*width+col)*channels + 3] = pixels[row][col].a;
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
    ImageSpec spec (width, height, channels, TypeDesc::UINT8);

    // Open output image file
    out->open(outImage, spec);

    // Write output image to disk and close
    out->write_image(TypeDesc::FLOAT, &oiioPixels[0]);
    out->close();
    delete out;
}

int main(int argc, char** argv){
	if (argc != 3)
    {
        printf("Usage: %s input_image output_image\n", argv[0]);
        return 1;
    }
    
    readImage(argv[1]);
    processImage();
    writeImage(argv[2]);

	return 0;
}