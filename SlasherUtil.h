#ifndef SLASHERUTIL_H
#define SLASHERUTIL_H

void minmax (int* val, int min, int max) ;
void minmaxf (float* val, float min, float max);
int signof(int x) ;
int randoValue(int min, int max);
void boundme (int* val, int min, int max);
int round(float x);

#endif