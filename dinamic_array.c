

#include "dinamic_array.h"

void initArray(Array *a, size_t initialSize) {
    a->first_time_arr = malloc(initialSize * sizeof(int));
    a->second_time_arr = malloc(initialSize * sizeof(int));
    a->index = 0;
    a->size = initialSize;
}

void insertArray(Array *a, float first_element, float second_element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore, a->used can go up to a->size
    if (a->index == a->size) {
        a->size *= 2;
        a->first_time_arr = realloc(a->first_time_arr, a->size * sizeof(float));
        a->second_time_arr = realloc(a->second_time_arr, a->size * sizeof(float));
    }
    a->first_time_arr[a->index] = first_element;
    a->second_time_arr[a->index] = second_element;
    a->index++;
}


float firstElementsAverage(Array *a) {
    float sum = 0;
    for (int i = 0; i < a->index; ++i) {
        sum += a->first_time_arr[i];
    }
    return sum / (float) (a->index);
}

float secondElementsAverage(Array *a) {
    float sum = 0;
    for (int i = 0; i < a->index; ++i) {
        sum += a->second_time_arr[i];
    }
    return sum / (float) (a->index);
}

float allElementsAverage(Array *a) {
    return (firstElementsAverage(a) + secondElementsAverage(a)) / 2;
}
void printTimes(Array *a){
    printf("All times:\n");
    for (int i = 0; i < a->index; ++i) {
        printf("The %d time the first time is: %f, second is: %f.\n",i+1,a->first_time_arr[i],a->second_time_arr[i]);
    }
}


void freeArray(Array *a) {
    free(a->first_time_arr);
    free(a->second_time_arr);
    a->first_time_arr = NULL;
    a->second_time_arr = NULL;
    a->index = 0;
    a->size = 0;
}