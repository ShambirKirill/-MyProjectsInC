#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/open_read_file.h"

#define MAX_COLUMNS 1024

char** read_file(char *file_name) {
    FILE *file;
    char ** lines_of_text = NULL;
    if ((file = fopen(file_name, "rb"))) {
        fseek(file , 0 , SEEK_END);
        long size_file = ftell(file);
        rewind(file);

        char * str = (char*)malloc(size_file*sizeof(char));
        if (str == NULL) {
            printf("malloc error. Terminated");
        } else {
            fread(str, 1, size_file, file);

            lines_of_text = text_processing(str, size_file);
            free(str);
            fclose(file);
        }
    }
    return lines_of_text;
}

char ** text_processing(char* str, int str_len) {
    int lines = 1;

    int line = 1;
    int str_c = 0;

     for (int i = 0; i < str_len; i++) { // I determine how many lines should be highlighted
        if (str[i] == 10) {
            lines++;
        }
    } 
    lines++;
    char **strings = NULL;
    strings = (char **)malloc(lines * MAX_COLUMNS * sizeof(char) + lines * sizeof(char *));
    if (strings == NULL) {
        printf("malloc error. Terminated");
    } else {
        char *point = (char *)(strings + lines);

        for (int i = 0; i < lines; i++) {
            *(strings + i) = point + MAX_COLUMNS * i;
            memset(*(strings + i), 0, MAX_COLUMNS);
        }

        sprintf(strings[0], "%d", lines); // The first line will store the number of lines

        for (int i = 0; i < str_len; i++) { // Write to the created array
            if (str_c == MAX_COLUMNS) {
                i = str_len;
                sprintf(strings[0], "%d", 0);
            } else {
                strings[line][str_c] = str[i];
                str_c++;
                if (str[i] == 10) {
                    strings[line][str_c+1] = '\0';
                    line++;
                    str_c = 0;
                }
            }
        }
    }
    return strings;
}
