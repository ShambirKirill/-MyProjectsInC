#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./array_processing.h"

void sort_array(int* array, int size) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size-1; j++) {
            if(array[j] > array[j+1]) {
                int tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

void unique(int* array, int size) {
    int* unique_array = (int*) calloc(size+1, sizeof(int));
    int index = 0;
    if(unique_array != NULL) {
        for (int i = 0; i < size; i++) {
            int match = 0;
            for (int u = 0; u < index+1; u++) {
                if (array[i] == unique_array[u]) {
                    match++;
                }
            }
            if (match == 0) {
                unique_array[index] = array[i];
                index++;
            }
        }
        unique_array[index] = -1;
        memset(array, 0, sizeof(int)*size);
        memcpy(array, unique_array, sizeof(int)*(index+1));
        sort_array(array, index);
        free(unique_array);
    }
}
