#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <iomanip>

/*
 * CP SC 212 P2: process.cpp
 * Created on: Mar 8, 2013
 * Author: Joshua Hull (jhull@clemson.edu)
 */

using namespace std;

// Iterate through an array of five coorelations and find the largest one.
int
getMaxCorrelation(double *correlations)
{
  int max = 0;

  for (int i = 1; i < 5; i++)
    if (correlations[i] > correlations[max])
      max = i;

  return max;
}


// Take two arrays of word lenght count and return the correlation between them.
double
getCorrelation(int *x, int *y)
{
  double sumxy = 0;
  double sumx = 0;
  double sumxSquared = 0;
  double sumy = 0;
  double sumySquared = 0;
  double correlation = 0;

  for (int i = 0; i < 25; i++)
    {
      sumxy = sumxy + (x[i] * y[i]);
      sumx = sumx + x[i];
      sumy = sumy + y[i];
      sumxSquared = sumxSquared + (x[i] * x[i]);
      sumySquared = sumySquared + (y[i] * y[i]);
    }

  correlation = (25 * (sumxy) - sumx * sumy)
                / (sqrt(25 * sumxSquared - (sumx * sumx))
                   * sqrt(25 * sumySquared - (sumy * sumy)));

  return correlation;
}

/* Strip a word of any punctuation.
 * Note: If any of these happen in the middle of a word it will only return the
 * part of the word before the punctuation.
 */
string
strip(string token)
{
  unsigned int period = token.find('.');
  unsigned int exclamation = token.find('!');
  unsigned int question = token.find('?');
  unsigned int comma = token.find(',');

  if (period > 25 && exclamation > 25
      && question > 25 && comma > 25)
    return token;

  else
    return token.substr(0, token.length() - 1);
}

// All the heavy lifting happens here.
void
process_language()
{
  int knownNdx = 0;

  int knownStats[5][25];  /* Five arrays of word length count for each of the
                           * five known languages.
                           */

  int workingStats[25];   // Word lenght count for the array we are processing.

  // Initialize the two arrays above to all zero.
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 25; j++)
      {
        knownStats[i][j] = 0;
        workingStats[i] = 0;
      }

  // The names of our finve known arrays.
  string knownNames[5] = {"","","","",""};

  // The correlations of our working array to the five knowns.
  double knownCorrelations[5] = {0,0,0,0,0};

  // The number of the most likely language.
  int likely = 0;

  // Whether we are processing a known or unknown language.
  bool unknown = false;

  // The name of the language we are processing.
  string workingName = "";

  // Keep going until we hit the end of the input.
  while (!cin.eof())
    {
      // Read in a word and strip it of any punctuaiton.
      string token, word;
      cin >> token;
      word = strip(token);

      // if our "word" is the special charater...
      if (word.compare("//") == 0)
        {
          // Read in the next word.
          cin >> token;
          word = strip(token);

          // If we are done with a language...
          if (!word.compare("End"))
            {
              // ...and it was an unkown...
              if (unknown)
                {
                  /* Print out the output of that language before moving
                   * on to the next.
                   */
                  cout << "Text " << workingName << endl;

                  for (int i = 0; i < 5; i++)
                    {
                      int *x = knownStats[i];
                      int *y = workingStats;


                      /* Get the correlation between the working language and
                       * each of the five unknowns.
                       */
                      knownCorrelations[i] = getCorrelation(x, y);

                      // Print out the correlation.
                      cout << "\tCorrelation with ";
                      cout << setw(10) << left << knownNames[i]
                           << " : ";

                      cout << setw(6) << setiosflags(ios::right);
                      cout << setprecision(2) << fixed << knownCorrelations[i]
                           << endl;
                    }

                  /* Figure out what the working lannguage most likely was
                   * and print it.
                   */
                  likely = getMaxCorrelation(knownCorrelations);

                  cout << workingName << " was most likely written in "
                       << knownNames[likely] << "." << endl << endl;
                }
            
        

              else
                {
                  /* We've reached the end of a known language. Simply
                   * increment the number of known languages.
                   */
                  knownNdx++;
                }
            }

          /* Here we are at the beginging of a section of text and we've
           * scanned in the name of a language.
           */
          else
            {
              /* If we haven't read in more then five languages yet then this
               * is just another known one.
               */
              if (knownNdx < 5)
                {
                  knownNames[knownNdx] = word;

                }

              /* If we have done all five known then this language is an
               * unknown one.
               */
              else
                {

                  unknown = true;

                  /* Initialize the stats back to zero and name our new
                   * unknown language.
                   */
                  for(int i = 0; i < 25; i++)
                    workingStats[i] = 0;

                  workingName = word;
                }
            }    
        }       

          // Here we just read in a word that is part of the text.
          else
            {
              /* If it is less then 25 characters long (remeber, we stripped off
               * punctuation)...
               */
              if(word.length() < 26)
                {
                  /* and this is an unkown language then inciriment the
                   * appropriate element in the unkown array.
                   */
                  if(unknown)
                    workingStats[word.length() - 1]++;

                  else
                    // If this is unkown then increment inside that array.
                    knownStats[knownNdx][word.length() - 1]++;
                } 
        }
    }
}

  int
  main(int argc, char *argv[])
  {
    // Why the heck not.
    process_language();
  }

