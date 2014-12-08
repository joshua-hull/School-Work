#include "stdio.h"
void main(){
	int i, j;
	for(i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			
			if (i > 0){
				printf("%4d|",2*j+((i-1)*(2*10))+1);
				printf("%-4d ",2*j+((i)*(2*10)));
			} else{
				printf("%4d|",2*j+i);
				printf("     ");
			}
		}
		printf("\n");
	}
}