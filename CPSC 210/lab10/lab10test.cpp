// Cpsc 210: Lab 10: lab10test.cpp (c) 2012 Joshua Hull

#include <iostream>
#include "ray.h"

using namespace std;

int main(int argc, char *argv[]) {
  // Set output characteristics of real numbers
  cout.setf(ios::fixed, ios::floatfield);
  cout.precision(6);

  ray v0(0);
  cout << "\nv0: Should print: (0.000000, 0.000000, 0.000000)\n";
  v0.vPrint();
  ray v0a = v0.vUnit();
  cout << "\nv0a: Should print: (0.000000, 0.000000, 0.000000)\n";
  v0a.vPrint();

  ray v1;
  cout << "\nv1: Should print: (0.000000, 0.000000, 1.000000)\n";
  v1.vPrint();

  ray v2(1.000000, 2.000000, 3.000000);
  cout << "\nv2: Should print: (1.000000, 2.000000, 3.000000)\n";
  v2.vPrint();

  ray v3(1.000000);
  cout << "\nv3: Should print: (1.000000, 1.000000, 1.000000)\n";
  v3.vPrint();

  ray v4(v2);
  cout << "\nv4: Should print: (1.000000, 2.000000, 3.000000)\n";
  v4.vPrint();

  ray v5 = v4.vScale(10);
  cout << "\nv5: Should print: (10.000000, 20.000000, 30.000000)\n";
  v5.vPrint();

  double len = v5.vLen();
  cout << "\nv5 len: Should print: 37.416574\n";
  cout << len << endl;

  ray v6 = v4.vSum(v5);
  cout << "\nv6: Should print: (11.000000, 22.000000, 33.000000)\n";
  v6.vPrint();

  ray v7 = v6.vDiff(v5);
  cout << "\nv7: Should print: (1.000000, 2.000000, 3.000000)\n";
  v7.vPrint();

  ray v8(5.000000, 10.000000, 15.000000);
  cout << "\nv7 dot v8: Should print: 70.000000\n";
  cout << v7.vDot(v8) << endl;

  cout << "\nv9: Should print: (0.267261, 0.534522, 0.801784)\n";
  ray v9 = v8.vUnit();
  v9.vPrint();

  double x = v9.getx();
  double y = v9.gety();
  double z = v9.getz();
  cout << "\nv9 gets: Should print: <0.267261, 0.534522, 0.801784>\n";
  cout << "<" << x << ", " << y << ", " << z << ">\n";

  /* Now implement a segment of code that is similar to the computations
     in plane_hits
  */
  double t;
  // Step 1: create the rays: 
  //           P set to <0, -2, -5>
  //           N set to <0, 0.5, 0.5>
  //           V set to <0, 0, 4>
  //           D set to <0, 0, -2>
    ray P(0,-2,-5);
    ray N(0,.5,.5);
    ray V(0,0,4);
    ray D(0,0,-2);

  // Step 2: unitize N and D
    N = N.vUnit();
    D = D.vUnit();

  // Step 3: compute "t" as (N dot P - N dot V) / (N dot D)
    t = (N.vDot(P) - N.vDot(V))/(N.vDot(D));

  // Step 4: set the ray H to the sum of V and D scaled by t
    ray H = D.vScale(t);
    H = H.vSum(V);

  // Step 5: OUTPUT and test the "z" coordinate of "H", set "t" to -1
  //         if positive.
    cout << "\nhitpoint z: Should print: H.z=-7.000000\n";
    if (H.getz() > 0) {
        t = -1;
    }
    cout << "H.z = " << H.getz() << endl;

  // Step 6: output "t" and "H"
    cout << "\nplaneHit: Should print: t=11.000000, "
       << "H=(0.000000, 0.000000, -7.000000)\n";
    cout << "t=" << t << ", H=";
    H.vPrint();

}
