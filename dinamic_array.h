
#include <malloc.h>

typedef struct Array {
    double *first_time_arr;
    double *second_time_arr;
    int freeIndex;
    size_t size;
} Array;

//initialize the dinamic array
int initArray(Array *a, size_t initialSize);

//insert elements and handle out of space
int insertArray(Array *a, double first_element, double second_element);

//all the average calculates and printing
double firstElementsAverage(Array *a);

double secondElementsAverage(Array *a);

double allElementsAverage(Array *a);

void printTimes(Array *a);
//free all the assigned
void freeArray(Array *a);