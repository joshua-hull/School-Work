// Cpsc 210: Lab 11: lab11test.cpp (c) 2012 Joshua Hull
#include <iostream>
#include <math.h>
#include "ray.h"
using namespace std;

int main () {
   // Set output characteristics of real numbers
   cout.setf(ios::fixed, ios::floatfield);
   cout.precision(0);

   // default constructor
   ray v0;
   ray v1 (1, 2, 3);

   cout << "Should be (1, 2, 3): " << endl;
   cout << v1;

   // copy constructor
   ray v2 = v1;

   cout << "Should be (1, 2, 3): " << endl;
   cout << v2;

   // equals operator
   v0 = v1 = v2;

   cout << "Should be (1, 2, 3): " << endl;
   cout << v0;

   // vScales v1 by 2
   ray v3 = v1 * 2;

   cout << "Should be (2, 4, 6): " << endl;
   cout << v3;

   ray v5(5, 5, 5);
   v3 = v1 - v5;
   cout << "Should be (-4, -3, -2): " << endl;
   cout << v3;

   // calls the subscript operator
   v3[0] = v3[1] = v3[2];
   cout << "Should be (-2, -2, -2): " << endl;
   cout << v3;

  /* MODIFY THE LINES WITH THE TAG "<<< CHANGE!" TO USE
     YOUR NEW OVERLOADED OPERATOR METHODS
  */
  double t;
  // Step 1: create the rays: 
  //           P set to <0, -2, -5>
  //           N set to <0, 0.5, 0.5>
  //           V set to <0, 0, 4>
  //           D set to <0, 0, -2>
  ray P(0, -2, -5);
  ray N(0, 0.5, 0.5);
  ray V(0, 0, 4);
  ray D(0, 0, -2);
  // Step 2: unitize N and D
  D = D.vUnit();
  N = N.vUnit();
  // Step 3: compute the double "t" as (N vDot P - N vDot V) / (N vDot D)
  t = (N.vDot(P) - N.vDot(V))/N.vDot(D);
  // Step 4: set the ray H to the sum of V and D scaled by t
  ray H = V.vSum(D.vScale(t));         // <<< CHANGE!
  // Step 5: OUTPUT and test the "z" coordinate of "H", set "t" to -1
  //         if positive.
  cout << "\nShould print: H.z=-7.000000\n";
  cout.precision(6);
  cout << "H.z =" << H[2] << endl;
  if (H[2] > 0) {
     t = -1;
  }
  // Step 6: output "t" and "H"
  cout << "\nShould print: t=11.000000, H=(0.000000, 0.000000, -7.000000)\n";
  cout << "t=" << t << ", H=";
  cout << H;}
