#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vecmat/Matrix.h"

void lowercase(char *s);
void process_input(Matrix3x3 &M, int width, int height);
void Rotate(Matrix3x3 &M, float theta);
void Scale(Matrix3x3 &M, float sx, float sy);
void Translate(Matrix3x3 &M, float dx, float dy);
void Flip(Matrix3x3 &M, int xf, int xy, int width, int height);
void Shear(Matrix3x3 &M, float hx, float hy);
void Perspective(Matrix3x3 &M, float px, float py);
