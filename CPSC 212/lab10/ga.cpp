/* Joshua Hull (jhull@clemson.edu)
 * CPSC 212: Lab 10: GA
 */

#include <cmath>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#define PERCENT_CROSS 90
#define POPULATION 2048

using namespace std;

int N;

class city
{
    public:
        int m_x;
        int m_y;
        city(int n, int x, int y);
        int m_n;
        double getDistance(city otherCity);
};

city::city(int n, int x, int y)
{
    m_n = n;
    m_x = x;
    m_y = y;
}

double city::getDistance(city otherCity)
{
    return sqrt(pow(otherCity.m_x - m_x, 2) + pow(otherCity.m_y - m_y, 2));
}

class trip
{
    private:
        int numCities;
    public:
        city* cities;
        trip(int n);
        ~trip();
        string getPath();
        double getTotalDistance();
        void print(double time);
        void add(int i, city c);
};

trip::trip(int n)
{
    cities = (city*) malloc(n * sizeof(city));
    numCities = n;
}

trip::~trip()
{
    free(cities);
}

string trip::getPath()
{
    ostringstream pathName;

    for(int i = 0; i < numCities; i++) {
        pathName << cities[i].m_n << " ";
    }

    return pathName.str();
}

double trip::getTotalDistance()
{
    double runningTotal = 0.0;

    for (int i = 0; i < numCities - 1; i++) {
        runningTotal = runningTotal + cities[i].getDistance(cities[i + 1]);
    }

    runningTotal = runningTotal + cities[numCities].getDistance(cities[0]);
    return runningTotal;
}

void trip::print(double time)
{
    cout << "Tour:\t" << getPath() << endl;
    cout << "Cost:\t" << getTotalDistance() << endl;
    cout << "Limit:\t" << time << endl;
}

void trip::add(int i, city c)
{
    if(i >= numCities)
        return;

    cities[i] = c;
}
void crossBread(trip** m_pop)
{
    int rand1, rand2;
    int crossOver = N/2;

    rand1 = rand() % POPULATION;
    rand2 = rand() % POPULATION;


    // Get the two parents and generate two offspring.
    trip* trip1 = m_pop[rand1];
    trip* trip2 = m_pop[rand2];

    trip* newTrip1 = new trip(N);
    trip* newTrip2 = new trip(N);


    // Populate the "x" chromosone.
    for(int i = 0; i < crossOver; i ++) {
        newTrip1->add(i, trip2->cities[i]);
        newTrip2->add(i, trip1->cities[i]);
    }

    bool isIn1 = false;
    bool isIn2 = false;

    // For each element in the "y" chromosone...
    for(int i = crossOver; i < N; i++) {
        // ...and for each element in the "other" parent...
        for(int j = 0; j < N; j++) {
            // ...search to see which of the first elements to add.
            for(int k = 0; k < i; k++) {
                if(trip1->cities[j].m_n == newTrip1->cities[k].m_n) {
                    isIn1 = true;
                }

                if(trip2->cities[j].m_n == newTrip2->cities[k].m_n) {
                    isIn2 = true;
                }

            }

            if(!isIn1) {
                newTrip1->add(i, trip1->cities[j]);
            }

            if(!isIn2) {
                newTrip2->add(i, trip2->cities[j]);
            }

            isIn1 = false;
            isIn2 = false;

        }
    }

    // Check to see if the new offspring are better then one of the parents.
    // If so, replace them.
    if(newTrip1->getTotalDistance() < trip1->getTotalDistance()) {
        m_pop[rand1] = newTrip1;
    }

    if(newTrip2->getTotalDistance() < trip2->getTotalDistance()) {
        m_pop[rand2] = newTrip2;
    }

}

void mutate(trip** m_pop)
{

    int randSpot = rand() % POPULATION;
    int randA = rand() % N;
    int randB = rand() % N;

    // Genetate a copy that we can mutate.
    trip* mutant = new trip(*m_pop[randSpot]);

    // Swap two random elements.
    city tmp = mutant->cities[randA];
    mutant->cities[randA] = mutant->cities[randB];
    mutant->cities[randB] = tmp;

    // If the mutated verson is better then replace the original.
    if(mutant->getTotalDistance() < m_pop[randSpot]->getTotalDistance()) {
        m_pop[randSpot] = mutant;
    }
}

int main(int argc, char* argv[])
{
    // Time related variables.
    clock_t start = clock();
    double timeLimit;
    double duration;

    // Seen the pseudorandom generator with the current time.
    srand(clock());

    // Stringbuilding varables.
    string fileLine;
    string stringN;
    string cityName;
    string tmp;

    // Citybuilding and journeybuilding variables.
    int cityX;
    int cityY;
    city* cities;
    trip** population;

    int* permiutation;

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " inputfile seconds" << endl;
        return 0;
    }

    ifstream inputFile(argv[1]);
    timeLimit = strtod(argv[2], NULL);

    if(!inputFile.is_open()) {
        cout << "Unable to open input file: " << argv[1] << endl;
        return 0;
    }

    // Get the number of cities to travel through from our input file.
    getline(inputFile, fileLine);
    stringstream lineStream(fileLine);
    lineStream >> stringN;
    N = atoi(stringN.c_str());

    /* Make a working trip and a best trip based on this number.
     * Also make the permiutations array and an array of cities.
     */
    population = (trip**) malloc(POPULATION * sizeof(trip));
    permiutation = (int*) malloc(N * sizeof(int));
    cities = (city*) malloc(N * sizeof(city));

    /* Populate the array of cities with info from the input file
     * as well as the permiutation array.
     */
    for(int i = 0; i < N; i++) {
        permiutation[i] = i;

        getline(inputFile, fileLine);
        stringstream lineStream(fileLine);
        lineStream >> cityName;

        lineStream >> tmp;
        cityX = atoi(tmp.c_str());

        lineStream >> tmp;
        cityY = atoi(tmp.c_str());

        city tmpCity(atoi(cityName.c_str()), cityX, cityY);
        cities[i] = tmpCity;

    }

    // All the examples on permiutations I found had this.
    sort(permiutation, permiutation + N);

    // Our initial population will be simple permiutations.
    for(int i = 0; i < POPULATION; i++) {
        next_permutation(permiutation, permiutation + N);
        trip* tmpTrip = new trip(N);

        for(int j = 0; j < N; j ++) {
            tmpTrip->add(j, cities[permiutation[j]]);
        }

        population[i] = tmpTrip;
    }

    do {
        duration = (double) (clock() - start) / (double)CLOCKS_PER_SEC;

        for(int i = 0; i < PERCENT_CROSS && duration < timeLimit; i++) {
            crossBread(population);
        }

        duration = (double) (clock() - start) / (double)CLOCKS_PER_SEC;


        for(int i = PERCENT_CROSS; i < 100 && duration < timeLimit; i++) {
            mutate(population);
        }

        duration = (double) (clock() - start) / (double)CLOCKS_PER_SEC;
    } while(duration < timeLimit);

    // Repate the whole process until we run out of time. Double check as we go along too.

    // Print the best journey and exit.
    trip* bestTrip = population[0];

    // Search the population array in a linear fashon to find the best one.
    for(int i = 0; i < POPULATION; i++) {
        if(population[i]->getTotalDistance() < bestTrip->getTotalDistance()) {
            bestTrip = population[i];
        }
    }

    ofstream dotFile;
    dotFile.open("ga.dot");

    dotFile << "digraph g {\n" << endl;
    for (int i = 0; i < N-1; i++){
            dotFile << "\t" << bestTrip->cities[i].m_n << " -> " << bestTrip->cities[i+1].m_n << " [penwidth=3, color=black]" << endl;
    }
    dotFile << "\t" << bestTrip->cities[N-1].m_n << " -> " << bestTrip->cities[0].m_n << endl;

    for (int i = 0; i < N; i++){
            dotFile << "\t" << bestTrip->cities[i].m_n << " [\n\t\tlabel = " << bestTrip->cities[i].m_n << "\n\t\tpos = \"" << bestTrip->cities[i].m_x << "," << bestTrip->cities[i].m_y << "!\"\n\t\tstyle=filled\n\t\tfillcolor=black\n\t]" << endl;
    }
    dotFile << "}" << endl;

    bestTrip->print(timeLimit);
    return 1;
}
