#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <OpenImageIO/imageio.h>
#include "pixel.h"

OIIO_NAMESPACE_USING

// Global variables to keep things a little simpler
int width;                              // Image width
int height;                             // Image height
int channels;                           // Number of channels in the image
rgba_pixel** pixels;      				// The actual pixels of the image
static std::vector<float> oiioPixels;	// OpeImageIO copy of the pixels

// Parameters for adjusting the masking. Set in processParameters
float minHue;
float maxHue;
float minSaturation;
float maxSaturation;
float minValue;
float maxValue;

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
	    	pixels[row][col].a = 1.0; // We'll change this in processImage()
	    }
}

void writeImage(char *outImage){

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

// Copied from assignment handout
#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))
void RGBtoHSV(int r, int g, int b, double &h, double &s, double &v){
	double red, green, blue;
	double max, min, delta;
	red = r / 255.0; green = g / 255.0; blue = b / 255.0; /* r , g, b to 0 − 1 scale */
	max = maximum(red, green, blue);
	min = minimum(red, green, blue);
	v = max; /* value i s maximum of r , g, b */
	if (max == 0) { /* saturation and hue 0 if value is 0 */
		s = 0;
		h = 0;
	} else {
		s = (max - min) / max; /* saturation i s colo r pu ri ty on scale 0 − 1 */
		delta = max - min;
		if (delta == 0) { /* hue doesn ’ t matter i f saturation i s 0 */
			h = 0;
		} else {
			if (red == max) { /* otherwise , determine hue on scale 0 − 360 */
				h = (green - blue) / delta;
			} else if (green == max) {
				h = 2.0 + (blue - red) / delta;
			} else {/* ( blue == max) */
				h = 4.0 + (red - green) / delta;
			}
			h = h * 60.0;
			if(h < 0) {
				h = h + 360.0;
			}
		}
	}
}

void processImage(){
	for (int row = 0; row < height; row++)
	    for (int col = 0; col < width; col++){
	    	double h;
	    	double s;
	    	double v;
	    	RGBtoHSV((int)(pixels[row][col].r*255),(int)(pixels[row][col].g*255),(int)(pixels[row][col].b*255), h, s, v);
	    	if(h > minHue && h < maxHue) {
	    		pixels[row][col].a = 0.0;
	    	}
	    }
}

void processParameters(){
	std::ifstream parametersFile;
  	parametersFile.open("parameters.txt");
  	if (parametersFile.is_open()){
  		parametersFile >> minHue >> maxHue >> minSaturation >> maxSaturation >> minValue >> maxValue;
  	} else {
  		std::cout << "ERROR opening parameters file" << std::endl;
  		exit(1);
  	}
}

int main(int argc, char** argv){
	if (argc != 3){
        printf("Usage: %s input_image output_image\n", argv[0]);
        return 1;
    }
    
    readImage(argv[1]);
    processParameters();
    processImage();
    writeImage(argv[2]);

	return 0;
}