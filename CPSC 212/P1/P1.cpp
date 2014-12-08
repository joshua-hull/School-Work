#include <iostream>
#include <fstream>
#include <malloc.h>
#include <iomanip>
#include <math.h>
/*
 * CPSC 212: P1
 * Joshua Hull
 */

using namespace std;

int main (int argc, char *argv[]) {

    // Number of integers present in the file.
    int num;

    // Pointer for array of integers.
    int *numbers;

    // Values to calculate from the array.
    int min;
    int max;
    double avg;
    double stdDev;

    // Need for avg and sdtDev calculations
    double avgSum;
    double stdDevSum;

    // Make sure we were given a file argument on the command line.
    if (argc != 2) {
        cout << "Error: No input file given. Exiting." << endl;
        return(0);
    }


    ifstream input;
    input.open(argv[1]);

    // Make sure we were able to open it.
    if(!input.good()){
        cout << "Error opening file." << endl;
        return(0);
    }

    // Read in number of integers to allocate space for.
    input >> num;
    numbers = (int *)malloc(num*sizeof(int));

    // Make sure our array was allocated.
    if(numbers == NULL) {
        cout << "Error mallocing numbers array. Exiting." << endl;
        return(0);
    }

    // If there are no integers then just print out message and quit.
    if(num == 0) {
        cout << "Zero integers present.\nMax: N/A\nMin: N/A\nAverage: N/A" << endl;
        cout << "Standard Deviation: N/A" << endl;
        return(1);
    }

    // Read in the integers into the array and close the file when done.
    for(int i = 0; i < num; i++){
        input >> numbers[i];
    }
    input.close();

    // Set default min and max and then determin the real min and max for the
    // array. Also keep a running sum for the average.
    min = numbers[0];
    max = numbers[0];
    for(int i = 0; i < num; i++){
        if(numbers[i] <= min)
            min = numbers[i];
        if(numbers[i] >= max)
            max = numbers[i];
        avgSum = avgSum + numbers[i];
    }

    // Print out the min and max.
    cout << "Max: " << max << "\nMin: " << min << "\n";

    // Average and Standard Deviation both use 2 decimal places.
    std::cout << std::setprecision(2) << std::fixed;

    // Calculate the sum and print it out.
    avg = avgSum / (double) num;
    cout << "Average: " << avg << endl;

    // Calculate the Standard Deviation and print it.
    for(int i = 0; i < num; i++){
        stdDevSum = stdDevSum + ( (numbers[i] - avg) * (numbers[i] - avg) );
    }
    stdDev = sqrt(stdDevSum/((double)num -1.0));
    cout << "Standard Deviation: "<< stdDev << endl;
    
    // All done.
    return(1);
}
