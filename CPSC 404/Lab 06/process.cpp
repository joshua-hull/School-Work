
#include "process.h"

void lowercase(char *s){
   int i;

   if(s != NULL) {
      for(i = 0; s[i] != '\0'; i++) {
         if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] += ('a' - 'A');
      }
   }
}

void Perspective(Matrix3x3 &M, float px, float py) {
  int row, col;
  Matrix3x3 R(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);

  R[3][0] = px;
  R[3][1] = py;

  Matrix3x3 Prod = R * M;

  for(row = 0; row < 3; row++) {
     for(col = 0; col < 3; col++) {
        M[row][col] = Prod[row][col];
     }
  }
}

void Shear(Matrix3x3 &M, float hx, float hy) {
  int row, col;
  Matrix3x3 R(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);

  R[0][2] = hx;
  R[1][0] = hy;

  Matrix3x3 Prod = R * M;

  for(row = 0; row < 3; row++) {
     for(col = 0; col < 3; col++) {
        M[row][col] = Prod[row][col];
     }
  }
}

void Flip(Matrix3x3 &M, int xf, int yf, int width, int height) {
  int row, col;
  Matrix3x3 R(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);

  if(xf == 1) {
    R[0][0] = -1;
    R[0][2] = width/2.0;
  }

  if(yf == 1) {
    R[1][1] = -1;
    R[1][2] = height/2.0;
  }

  Matrix3x3 Prod = R * M;

  for(row = 0; row < 3; row++) {
     for(col = 0; col < 3; col++) {
        M[row][col] = Prod[row][col];
     }
  }
}

void Translate(Matrix3x3 &M, float dx, float dy) {
  int row, col;
  Matrix3x3 R(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);

  R[0][2] = dx;
  R[1][2] = dy;

  Matrix3x3 Prod = R * M;

  for(row = 0; row < 3; row++) {
     for(col = 0; col < 3; col++) {
        M[row][col] = Prod[row][col];
     }
  }
}

void Scale(Matrix3x3 &M, float sx, float sy) {
  int row, col;
  Matrix3x3 R(1.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0);

  R[0][0] = sx;
  R[1][1] = sy;

  Matrix3x3 Prod = R * M;

  for(row = 0; row < 3; row++) {
     for(col = 0; col < 3; col++) {
        M[row][col] = Prod[row][col];
     }
  }
}

void Rotate(Matrix3x3 &M, float theta){
   int row, col;
   Matrix3x3 R(1.0, 0.0, 0.0,
               0.0, 1.0, 0.0,
               0.0, 0.0, 1.0);
   double rad, c, s;

   rad = PI * theta / 180.0;
   c = cos(rad);
   s = sin(rad);

   R[0][0] = c;
   R[0][1] = -s;
   R[1][0] = s;
   R[1][1] = c;

   Matrix3x3 Prod = R * M;

   for(row = 0; row < 3; row++) {
      for(col = 0; col < 3; col++) {
         M[row][col] = Prod[row][col];
      }
   }
}

void process_input(Matrix3x3 &M, int width, int height){
   char command[1024];
   bool done;
   float theta;
   float sx, sy;
   float dx, dy;
   int xf, yf;
   float hx, hy;
   float px, py;


   /* build identity matrix */
   M.identity();

   for(done = false; !done;) {

      /* prompt and accept input, converting text to lower case */
      printf("> ");
      scanf("%s", command);
      lowercase(command);

      /* parse the input command, and read parameters as needed */
      if(strcmp(command, "d") == 0) {
         done = true;
      } else if(strlen(command) != 1) {
         printf("invalid command, enter r, s, t, f, h, p, d\n");
      } else {
         switch(command[0]) {

            case 'r':		/* Rotation, accept angle in degrees */
               if(scanf("%f", &theta) == 1)
                  Rotate(M, theta);
               else
                  fprintf(stderr, "invalid rotation angle\n");
               break;
            case 's':		/* Scale, accept scale factors */
               if(scanf("%f %f", &sx, &sy) == 2 && !(sx == 0 || sy == 0))
                  Scale(M, sx, sy);
               else
                  fprintf(stderr, "invalid scale factors\n");
               break;
            case 't':		/* Translation, accept translations */
               if(scanf("%f %f", &dx, &dy) == 2)
                  Translate(M, dx, dy);
               else
                  fprintf(stderr, "invalid translation factors\n");
               break;
            case 'f':		/* Mirror, accept 0/1, 0/1 for x mirror, ymirror */
               if(scanf("%d %d", &xf, &yf) == 2 && (xf == 0 || xf == 1) && (yf == 0 || yf == 1))
                  Flip(M, xf, yf, width, height);
               else
                  fprintf(stderr, "invalid flip factors\n");
               break;
            case 'h':		/* Shear, accept shear factors */
               if(scanf("%f %f", &hx, &hy) == 2)
                  Shear(M, hx, hy);
               else
                  fprintf(stderr, "invalid shear factors\n");
               break;
            case 'p':		/* Perspective, accept perspective factors */
               if(scanf("%f %f", &px, &py) == 2)
                  Perspective(M, px, py);
               else
                  fprintf(stderr, "invalid perspective factors\n");
               break;
            case 'd':		/* Done, that's all for now */
               done = true;
               break;
            default:
               printf("invalid command, enter r, s, t, f, h, p, d\n");
         }
      }
   }
}
