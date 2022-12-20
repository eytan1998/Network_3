
#include "dinamic_array.h"

int initArray(Array *a, size_t initialSize) {
    a->first_time_arr = NULL;
    a->second_time_arr = NULL;
    a->first_time_arr = malloc(initialSize * sizeof(double ));
    if (a->first_time_arr == NULL) {
        fputs("[-]memory alloc fails first_time_arr", stderr);
        free(a->first_time_arr);
        return -1;
    }

    a->second_time_arr = malloc(initialSize * sizeof(double ));
    if (a->second_time_arr == NULL) {
        fputs("[-]memory alloc fails of second_time_arr", stderr);
        free(a->second_time_arr);
        return -1;
    }
    a->freeIndex = 0;
    a->size = initialSize;
    return 0;
}

int insertArray(Array *a, double first_element, double second_element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore, a->used can go up to a->size
    if (a->freeIndex == a->size) {
        a->size *= 2;
        a->first_time_arr = realloc(a->first_time_arr, a->size * sizeof(double ));
        if (a->first_time_arr == NULL) {
            fputs("[-]memory alloc fails first_time_arr", stderr);
            free(a->first_time_arr);
            return -1;
        }
        a->second_time_arr = realloc(a->second_time_arr, a->size * sizeof(double ));
        if (a->second_time_arr == NULL) {
            fputs("[-]memory alloc fails of second_time_arr", stderr);
            free(a->second_time_arr);
            return -1;
        }
    }
    a->first_time_arr[a->freeIndex] = first_element;
    a->second_time_arr[a->freeIndex] = second_element;
    a->freeIndex++;
    return 0;
}

double firstElementsAverage(Array *a) {
    double sum = 0;
    for (int i = 0; i < a->freeIndex; ++i) {
        sum += a->first_time_arr[i];
    }
    return sum / (double ) (a->freeIndex);
}

double secondElementsAverage(Array *a) {
    double sum = 0;
    for (int i = 0; i < a->freeIndex; ++i) {
        sum += a->second_time_arr[i];
    }
    return sum / (double ) (a->freeIndex);
}

double allElementsAverage(Array *a) {
    return (firstElementsAverage(a) + secondElementsAverage(a)) / 2;
}

void printTimes(Array *a) {
    printf("\nAll times:\n");
    for (int i = 0; i < a->freeIndex; ++i) {
        printf("The #%d time: the first part is: %f, second is: %f millisecond\n", i + 1, a->first_time_arr[i],
               a->second_time_arr[i]);
    }
}

void freeArray(Array *a) {
    free(a->first_time_arr);
    free(a->second_time_arr);
    a->first_time_arr = NULL;
    a->second_time_arr = NULL;
    a->freeIndex = 0;
    a->size = 0;
}
