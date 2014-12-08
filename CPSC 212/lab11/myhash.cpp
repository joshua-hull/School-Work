#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <climits>

using namespace std;

int main(int argc, char* argv[])
{
    string tmpString, fileLine;

    /* I had to give it a ceiling and this seemed reasonable. Are there even
     * that many primes less then MAX_INT?
     */
    int primes[256];

    int numPrimes = 0;
    int* hashTable;

    if(argc != 2) {
        cout << "Usage: " << argv[0] << " inputfile" << endl;
        return 0;
    }

    // Open up the file.
    ifstream inputFile(argv[1]);

    // Make sure it is open.
    if(!inputFile.is_open()) {
        cout << "Unable to open input file!" << endl;
        return 0;
    }

    // Get the line of primes and create a stream based on it.
    getline(inputFile, fileLine);
    stringstream lineStream(fileLine);

    // Initialize the array.
    for(int i = 0; i < 256; i++) {
        primes[i] = 0;
    }

    // Keep reading in primes until there aren't any more.
    while(lineStream >> tmpString && numPrimes < 256) {
        primes[numPrimes] = atoi(tmpString.c_str());
        numPrimes++;

    }

    // Malloc our hashtable.
    hashTable = (int*) malloc(sizeof(int) * primes[0]);

    /* I don't know why I chose this versus -1 since we were guranteed positive
     * numbers. Oh well...
     */
    for (int i = 0; i < primes[0]; i++) {
        hashTable[i] = INT_MIN;
    }

    // Read in the line of primes and make a stream.
    getline(inputFile, fileLine);
    stringstream lineStream2(fileLine);

    // Keep reading until we run out of numbers to insert.
    while(lineStream2 >> tmpString) {
        int tmp = atoi(tmpString.c_str());
        int t = 0;
        int N = primes[0];

        // Keep going until we find an empty spot or loop back around.
        while(hashTable[((tmp + t) % N)] != INT_MIN && t < N) {
            t++;
        }

        // If we are an an empty slot.
        if(hashTable[(tmp + t) % N] == INT_MIN) {
            hashTable[((tmp + t) % N)] = tmp;
        }
        // Otherwise we looped and the table is full.
        else {
            cout << "Hash table full. Value not inserted." << endl;
        }


    }

    // Print out the table.
    for(int i = 0; i < primes[0]; i++) {
        cout << i << "\t";

        // If the slot is enpty then don't print a value for that key.
        if(hashTable[i] != INT_MIN) {
            cout << hashTable[i];
        }

        cout << endl;
    }

    return 1;
}
