#ifndef INPUTVALS_H
#define INPUTVALS_H

/** Prototype Statements **/
int readInt(FILE *infp, char *errmsg);
double readDouble(FILE *infp, char *errmsg);
pixel_t readColor(FILE *infp, char *errmsg);
triple_t readTriple(FILE *infp, char *errmsg);

#endif

