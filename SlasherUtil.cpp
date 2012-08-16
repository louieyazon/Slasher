#include <cmath>
#include <ctime>
#include <stdlib.h>

void minmax (int* val, int min, int max) {
	//this function keeps the value of the variable at address -val- between min and max
	if(*val > max) *val = max;
	if(*val < min) *val = min;
}
void minmaxf (float* val, float min, float max) {
	if(*val < min) *val = min;
	if(*val > max) *val = max;
}
int signof(int x) {
	if (x != 0) return (x / abs(x));
	else return 0;
}

int randoValue(int min, int max) {
	return (    (rand() % (max - min)) + min    );
}



int round(float x){
	return (int)(x + 0.5);
};