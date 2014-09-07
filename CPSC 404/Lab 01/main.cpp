/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Display an image given on the command line to an OpenGL window and write to
 * an optional output file given from the command line. Also allow the user to
 * click on the image in order to display the pixel information about the
 * location they clicked on via the terminal.
 *
 * Usage: oiioview input_image [output_image]
 */

#include <stdio.h>
#include <stdlib.h>
#include <OpenImageIO/imageio.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

OIIO_NAMESPACE_USING

// Global variables to keep things a little simpler
int width;                              // Image width
int height;                             // Image height
int channels;                           // Number of channels in the image
char *outImage;                         // File name of the output image
std::vector<unsigned char> pixels;      // The actual pixels of the image
bool canWrite;                          // Did we get an output name from the command line?

/**
 * OpenGL drawing callback
 */
void drawImage()
{
    //Reset position to origin
    glRasterPos2i(0, 0);

    // Dump the pixels
    if (channels == 3) glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
    else glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

    // Flush to screen.
    glFlush();
}

/**
 * Write the image to the file system given the command line parameter.
 * @param outImage Output file image name.
 */
void writeImage(char *outImage)
{
    // Flip the image from OpenGL's upside down version
    std::vector<unsigned char>rightSideUp(width * height * 3);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for (int k = 0; k < channels; k++)
                rightSideUp[((height - 1 - i) * width * channels) + (j * channels) + k] = pixels[(i * width * channels) + (j * channels) + k];

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
    out->write_image(TypeDesc::UINT8, &rightSideUp[0]);
    out->close();
    delete out;
}

/**
 * Read the image given on the command line from the disk.
 * @param inImage File name of the image to read in.
 */
void readImage(char *inImage)
{
    // Open image input
    ImageInput *in = ImageInput::open(inImage);

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

    // Create working copy of the pixels
    std::vector<unsigned char>upsideDown(width * height * channels);

    // Init the global copy of the pixels
    pixels = std::vector<unsigned char>(width * height * channels);

    // Read in the pixels and close the file
    in->read_image(TypeDesc::UINT8, &upsideDown[0]);
    in->close();
    delete in;

    // Invert the global copy of the pixels for OpenGL.
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for (int k = 0; k < channels; k++)
                pixels[((height - 1 - i) * width * channels) + (j * channels) + k] = upsideDown[(i * width * channels) + (j * channels) + k];
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
        if (canWrite) writeImage(outImage);
    default:
        return;
    }
}

/**
 * Handle mouse events. OpenGL mouse callback.
 * @param btn   Mouse button
 * @param state Click mode
 * @param x     x pos.
 * @param y     y pos.
 */
void handleMouse(int btn, int state, int x, int y)
{
    // Pixel at the location we clicked
    unsigned char *pix;
    pix = &pixels[((height - 1 - y) * width * channels) + (x * channels)];

    // Print out information
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) printf("(x,y) = (%3d,%3d)\tRGB = (%3u, %3u, %3u) \n", x, y, pix[0], pix[1], pix[2]);
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
    glutCreateWindow("oiioview - Joshua Hull (jhull@clemson.edu)");

    // Callback setup
    glutDisplayFunc(drawImage);
    glutKeyboardFunc(handleKey);
    glutMouseFunc(handleMouse);

    // More window setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glClearColor(1, 1, 1, 1);

}

int main (int argc, char **argv)
{
    // Parameter checking and assignment
    if (argc != 3 && argc != 2)
    {
        printf("Usage: %s input_image [output_image]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Image ouput setup
    if (argc == 3)
    {
        outImage = argv[2];
        canWrite = true;
    }

    // Read in the image
    readImage(argv[1]);

    // Init OpenGL
    glutInit(&argc, argv);
    openGLSetup(width, height);

    // Start running display window
    glutMainLoop();
    return EXIT_SUCCESS;
}