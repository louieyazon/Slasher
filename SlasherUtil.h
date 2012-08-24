#ifndef SLASHERUTIL_H
#define SLASHERUTIL_H

void minmax (int* val, int min, int max) ;
void minmaxf (float* val, float min, float max);
int signof(int x) ;
int randoValue(int min, int max);

int round(float x);
float CirclesIntersection(float posAX, float posAY, float radiusA, float posBX, float posBY, float radiusB);
bool AreCirclesIntersecting(float posAX, float posAY, float radiusA, float posBX, float posBY, float radiusB);

#endif