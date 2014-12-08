/* This program should read up to 10 numbers and print either
   the first negative value, that there are no negative values.

   Note: the program contains a bug that only shows up when it is
         run.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int ndx;
    float table[10];
    int count = 0;
    int search = 1;

    count = 0;
    while ((count < 10) && (scanf("%f", &table[count]) == 1)) {
       count++;
    }

    ndx = 0;
    while ((search = 1)) {
        if ((table[ndx++] < 0) || (count < ndx)) {
           search = 0;
        }
    }

    if (ndx <= count) {
       printf("First negative number=%f\n", table[ndx-1]);
    }
    else {
       printf("No negative numbers\n");
    }

    exit(0);
}
