/** broken3 -- run time error in code **/
#include <stdio.h>
#include <stdlib.h>

/** dumb **/
int dumb(int array[], int b) {
   int min = array[0];
   int x = array[1];
   int y = array[2];
   if (b < array[0]) {
      min = b;
   }
   array[1] = x;
   array[2] = y;
   return(min);
}
   
/** readmax **/
int readmax(int array[], int len) {
   int max;
   int first=1;
   int ndx;
   while (ndx < len) {
      printf("Enter value: ");
      if (scanf("%d", &array[ndx])<1) {
         printf("End of data\n");
         break;
      }
      if (first==1) {
          max = array[ndx];
          first = 0;
      }
      else
      if (array[ndx] > max) {
         max = array[ndx];
      }
      ndx++;
   }
   return(max);
}
   
/** main **/
int main() {
    int array[10] = {-100000, -200000, -300000, -400000, -500000,
                     -600000, -700000, -800000, -900000, -999999};
    int min, max;

    min = dumb(array, -99999);
    max = readmax(array, 10);
    printf("Min=%d\n", min);
    printf("Max=%d\n", max);

    return(0);
}
