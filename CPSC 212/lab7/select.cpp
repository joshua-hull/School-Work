#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

/*
 * CP SC 212 Lab 7: select.cpp
 * Name: Joshua Hull (jhull@clemson.edu)
 */

void
swap(int a, int b, int *A)
{
  // Swap values in positions a and b of Array A using tmp as a temporary place
  // holder.
  int tmp = A[a];
  A[a] = A[b];
  A[b] = tmp;
}

int
partition(int *A, int len, int pivot_index)
{
  // Get the value we're piviting against.
  int pivot_value = A[pivot_index];
  int ndx = 0;

  // Shove this value to the end of the array.
  swap(pivot_index, len - 1, A);

  // Walk throught the array.
  for (int i = 0; i < len; i++)
    {
      // ndx is where the pivot value will end up at the end. If we are less
      // then the pivot value then move to where ndx is and move ndx up one.
      if (A[i] < pivot_value)
        {
          swap(ndx, i, A);
          ndx++;
        }
    }

  // Put the pivot back where it needs to go and return where we put it: the
  // new pivot index.
  swap(len - 1, ndx, A);
  return ndx;
}

int
select(int *A, int len, int rank)
{
  if (len == 1)
    return A[0];

  int pivot_index = rand() % len;
  int pivot_rank = partition(A, len, pivot_index);

  if (rank == pivot_rank)
    return A[rank];
  // If what we are looking for is less then the pivot rank...
  if (rank < pivot_rank)
    // ...simply search a scaled down array that is to the right of the pivot rank
    return select(A, pivot_rank, rank);
  // Otherwise search the upper half of the array.
  return select(A + pivot_rank + 1, (len - pivot_rank) - 1,
      (rank - pivot_rank) - 1);
}

int
main(void)
{
  int N, *A;

  // Open data file and read in the number of elements.
  ifstream fin("data.txt");
  fin >> N;

  // Create an array of that size and read in the values that follow.
  A = new int[N];
  for (int i = 0; i < N; i++)
    fin >> A[i];

  // Close the input file.
  fin.close();

  // Read in the rank we are hunting for.
  int r;
  cout << "Enter rank (0.." << N - 1 << ")\n";

  // Keep reading in ranks and reporting values until EOF.
  while (cin >> r)
    {
      if (r < 0 || r >= N)
        cout << "Invalid rank\n";
      else
        cout << select(A, N, r) << "\n";
    }

  delete[] A;
}

