
#include <malloc.h>

typedef struct Array {
    float *first_time_arr;
    float *second_time_arr;
    int index;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize);

void insertArray(Array *a, float first_element, float second_element);

float firstElementsAverage(Array *a);

float secondElementsAverage(Array *a);

float allElementsAverage(Array *a);

void printTimes(Array *a);

void freeArray(Array *a);