
#include <malloc.h>

typedef struct Array {
    double *first_time_arr;
    double *second_time_arr;
    int freeIndex;
    size_t size;
} Array;

int initArray(Array *a, size_t initialSize);

int insertArray(Array *a, double first_element, double second_element);

double firstElementsAverage(Array *a);

double secondElementsAverage(Array *a);

double allElementsAverage(Array *a);

void printTimes(Array *a);

void freeArray(Array *a);