#include <stdio.h>
#include <string.h>

void grep_with_e(char *pattern, FILE *file, char* start_of_line);
void grep_with_i(char *pattern, FILE *file, char* start_of_line);
void grep_with_v(char *pattern, FILE *file, char* start_of_line);
void grep_with_c(char *pattern, FILE *file, char* start_of_line);
void grep_with_l(char *pattern, FILE *file, char* filename);
void grep_with_n(char *pattern, FILE *file, char* start_of_line);
int define_option(char *arg);
char * to_lower(char * orig_string, char * output_string);

int main(int argc, char * argv[]) {
    char start_of_line[124] = ""; // to print the file name if more than one file is transmitted
    if (argc > 3) {
        int option = define_option(argv[1]);
        for (int i = 3; i < argc; i++) {
            FILE *file;
            file = fopen(argv[i], "r");
            if (file != NULL) {
                if (argc > 4) {
                    strcpy(start_of_line, argv[i]);
                    start_of_line[strlen(start_of_line)] = ':';
                }
                switch(option) {
                    case 1:
                        grep_with_e(argv[2], file, start_of_line);
                        break;
                    case 2:
                        grep_with_i(argv[2], file, start_of_line);
                        break;
                    case 3:
                        grep_with_v(argv[2], file, start_of_line);
                        break;
                    case 4:
                        grep_with_c(argv[2], file, start_of_line);
                        if (i != argc-1) printf("\n");
                        break;
                    case 5:
                        grep_with_l(argv[2], file, argv[i]);
                        if (i != argc-1) printf("\n");
                        break;
                    case 6:
                        grep_with_n(argv[2], file, start_of_line);
                        if (i != argc-1) printf("\n");
                        break;
                }
            } else {
                printf("\ns21_grep: %s: No such file or directory\n", argv[i]);
            }
            fclose(file);
        }
    } else {
        printf("s21_grep [options] template [file_name]");
    }
    return 0;
}

int define_option(char *arg) {
    // The function determines which flag the entered key belongs to
    int result = 0;
    char flags[6][4] = {
        "-e", "-i", "-v", "-c",
        "-l", "-n"
    };
                
    for (int option = 0; option < 6; option++) {
        if (strcmp(arg, flags[option]) == 0) {
            result = option+1;
            break;
        }
    }
    return result;
}

void grep_with_e(char *pattern, FILE *file, char* start_of_line) { // Тоже самое что -v но наоборот, схлопнуть по итогу
    char str[1024];

    while (!feof(file)) {
        char *istr;
        fgets(str, 2054, file);
        istr = strstr(str, pattern);
        if (istr) {
            printf("%s%s\0", start_of_line, str);
        }
    }
}

void grep_with_i(char *pattern, FILE *file, char* start_of_line) {
    char str[1024];
    char lower_string[1024];
    char lower_pattern[1024];
    while (!feof(file)) {
        char *istr = NULL;
        fgets(str, 2054, file);
        istr = strstr(to_lower(str, lower_string), to_lower(pattern, lower_pattern));
        if (istr) {
            printf("%s%s\0", start_of_line, str);
        }
    }
}

void grep_with_v(char *pattern, FILE *file, char* start_of_line) { // Тоже самое что -e но наоборот, схлопнуть по итогу
    char str[1024];
    while (!feof(file)) {
        char *istr;
        fgets(str, 2054, file);
        istr = strstr(str, pattern);
        if (!istr) {
            printf("%s%s\0", start_of_line, str);
        }
    }
}

void grep_with_c(char *pattern, FILE *file, char* start_of_line) {
    char str[1024];
    int n_strings = 0;
    while (!feof(file)) {
        char *istr;
        fgets(str, 2054, file);
        istr = strstr(str, pattern);
        if (istr) {
            n_strings++;
        }
    }
    printf("%s%d", start_of_line, n_strings);
}

void grep_with_l(char *pattern, FILE *file, char *filename) {
    int exists = 0;
    char str[1024];
    while (!feof(file)) {
        char *istr;
        fgets(str, 2054, file);
        istr = strstr(str, pattern);
        if (istr) {
            exists++;
            break;
        }
    }
    if (exists) {
        printf(filename);
    }
}

void grep_with_n(char *pattern, FILE *file, char* start_of_line) {
    char str[1024];
    int n = 1;
    while (!feof(file)) {
        char *istr = NULL;
        fgets(str, 2054, file);
        istr = strstr(str, pattern);
        if (istr) {
            printf("%s%d:%s", start_of_line, n, str);
        }
        n++;
    }
}

char * to_lower(char * orig_string, char * output_string) {
    int i = 0;
    for (char *p = orig_string; *p; *p++, i++) {
        if (*(p) > 64 && *(p) < 91) {
            output_string[i] = *(p) + 32;
        } else {
            output_string[i] = *(p);
        }
    }
    output_string[i] = '\0';
    return output_string;
}
