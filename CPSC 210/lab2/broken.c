/* This program is supposed to read in a collection */
/* of pairs of integers, print each pair and then   */
/* print their average.                             */

/* Note: it is full of syntax errors                */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int x, y;
    float avg;

    while (scanf("%d %d", &x , &y) == 2)
    {
       printf("x = %d and y =%d \n", x, y);
       avg = ((float)x + (float)y)/2.0;
       printf("average is %f\n", avg);
    }
    return (0);
}
