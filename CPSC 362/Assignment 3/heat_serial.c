#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

void print_usage(){
	printf("heat_serial.exe [# of iterations] [room dimension]\n");
	exit(EXIT_FAILURE);
}

char* print_color(double temperature){
    if (temperature > 250.0) return RED;
    if (temperature > 180.0) return ORANGE;
	if (temperature > 120.0) return YELLOW;
	if (temperature > 80.0) return LTGREEN;
	if (temperature > 60.0) return GREEN;
	if (temperature > 50.0) return LTBLUE;
	if (temperature > 40.0) return BLUE;
	if (temperature > 30.0) return DARKTEAL;
	if (temperature > 20.0) return BROWN;
	return BLACK;
}

int main(int argc, char const *argv[]) {
	if(argc != 3) print_usage();
	if((atoi(argv[1]) < 0) || (atoi(argv[2]) < 0)) print_usage();

	int iterations = atoi(argv[1]);
	int dim = atoi(argv[2]);
	int i, j, k, left, right;
	float* old;
	float* new;

	old = calloc((dim+2)*(dim+2),sizeof(float));
	new = calloc((dim+2)*(dim+2),sizeof(float));

	left = (int) dim * .30;
	right = (int) dim * .70;

	for(i = 0; i < left + 1; i++){
		old[i] = 20.0;
		new[i] = 20.0;
	}
	for(i = left + 1; i <= right + 1; i++){
		old[i] = 300.0;
		new[i] = 300.0;
	}
	for(i=right+2; i <= dim +1; i++){
		old[i] = 20.0;
		new[i] = 20.0;
	}
	for (i = 1; i <= dim + 1; i++) {
		for (j = 0; j <= dim + 1; j++) {
			old[i*(dim+2)+j] = 20.0;
			new[i*(dim+2)+j] = 20.0;
		}
	}
	for(k=1; k<= iterations; k++){
        for (i = 1; i <= dim; i++) {
			for (j = 1; j <= dim; j++) {
				new[i*(dim+2)+j] = ((old[(i+1)*(dim+2)+j] + old[(i-1)*(dim+2)+j] + old[i*(dim+2)+(j+1)] + old[i*(dim+2)+(j-1)])/4);
            }
        }
		for (i = 0; i <= dim + 1; i++) {
			for (j = 0; j <= dim + 1; j++) {
				old[i*(dim+2)+j] = new[i*(dim+2)+j];
			}
		}
	}
    char filename[32];
    sprintf(filename,"output.pnm");
    FILE* fp;
    fp = fopen(filename,"w");
    fprintf(fp, "P3\n%d %d\n15\n",dim, dim);
    for (i = 1; i <= dim; i++) {
        for (j = 1; j <= dim; j++) {
            fprintf(fp, "%s ", print_color(new[i*(dim+2)+j]));
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
	free(old);
	free(new);
	return EXIT_SUCCESS;
}
