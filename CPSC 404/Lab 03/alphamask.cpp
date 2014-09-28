#include <cstdlib>
#include <stdio.h>

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