#include <stdio.h>
#include <string.h>

void cat_with_b(FILE *file);
void cat_with_e(FILE *file, int is_gnu);
void cat_with_n(FILE *file);
void cat_with_s(FILE *file);
void cat_with_t(FILE *file, int is_gnu);
int define_option(char *arg, int *is_gnu);
void non_printing(char *str, int is_t);
void n_space(int digit, char *space);

int main(int argc, char * argv[]) {
    int is_gnu = 0;
    if (argc > 2) {
        int index_argv = 0;
        int option;
        for (; index_argv < argc; index_argv++) {
            option = define_option(argv[index_argv], &is_gnu);
            if(option) {
                break;
            }
        }
        for (int i = 1; i < argc; i++) {
            if (i != index_argv) {
                FILE *file;
                file = fopen(argv[i], "r");
                if (file != NULL) {
                    switch(option) {
                        case 1:
                            cat_with_b(file);
                            break;
                        case 2:
                            cat_with_e(file, is_gnu);
                            break;
                        case 3:
                            cat_with_n(file);
                            break;
                        case 4:
                            cat_with_s(file);
                            break;
                        case 5:
                            cat_with_t(file, is_gnu);
                            break;
                    }
                } else {
                    printf("\ns21_cat: %s: No such file or directory\n", argv[i]);
                }
                fclose(file);
            }
        }
    } else {
        printf("cat [OPTION] [FILE]...");
    }
}

int define_option(char *arg, int *is_gnu) {
    int option;
    int result = 0;
    char flags[12][20] = {
        "-b", "--number-nonblank", "-e", "-E",
        "-n", "--number", "-s", "--squeeze-blank","-t", "-T"
    };
    for (int i = 0; i < 10; i++) {
        if (strcmp(arg, flags[i]) == 0) {
            option = i;
            if (option % 2 == 0) {
                *is_gnu = 0;
            } else {
                *is_gnu = 1;
            }
            if (option == 0 || option == 1) {
                result = 1;
            } else if (option == 2 || option == 3) {
                result = 2;
            } else if (option == 4 || option == 5) {
                result = 3;
            } else if (option == 6 || option == 7) {
                result = 4;
            } else if (option == 8 || option == 9) {
                result = 5;
            }
            break;
        }
    }
    return result;
}

void cat_with_b(FILE *file) {
    char str[1024];
    int line_num = 1;
    while (!feof(file)) {
        char space[6] = "     ";
        n_space(line_num, space);
        fgets(str, 2054, file);
        if (strlen(str) != 1) {
            printf("%s%d %s", space, line_num, str);
            line_num++;
        } else if (str[0] != 10) {
            printf("%s%d %s", space, line_num, str);
            line_num++;
        } else {
            printf("%s", str);
        }
    }
}

void cat_with_e(FILE *file, int is_gnu) {
    char str[1024];
    while (!feof(file)) {
        fgets(str, 2054, file);
        char *ach;
        ach = strrchr(str, '\n');
        int last_char_pos = ach-str+1;
        if(ach != NULL)
            str[last_char_pos-1] = '$';
            if (is_gnu == 0) {
                non_printing(str, 0);
                printf("\n");
            } else {
                printf("%s\n", str);
            }
    }
}

void cat_with_n(FILE *file) {
    char str[1024];
    int line_num = 1;
    while (!feof(file)) {
        char space[6] = "     ";
        n_space(line_num, space);
        fgets(str, 2054, file);
        printf("%s%d  %s", space, line_num, str);
        line_num++;
    }
}

void cat_with_s(FILE *file) {
    char str[1024];
    int mark_empty = 0;
    while (!feof(file)) {
        fgets(str, 2054, file);
        if (strlen(str) != 1) {
            printf(str);
            mark_empty = 0;
        } else if (str[0] == 10) {
            if (mark_empty == 0) {
                mark_empty = 1;
                printf(str);
            }
        } else {
            printf("%s", str);
        }
    }
}

void cat_with_t(FILE *file, int is_gnu) {
    char str[1024];
    while (!feof(file)) {
        fgets(str, 2054, file);
        if (is_gnu == 1) {
            for (int i = 0; *(str+i); i++) {
                if (str[i] != 9) {
                    printf("%c", str[i]);
                } else {
                    printf("^I");
                }
            }
        } else {
            non_printing(str, 1);
        }
    }
}

void non_printing(char *str, int is_t) {
    int control_char[6] = {
        11, 13, 12, 7,
        27, 8
    };
    int i = 0;
    for (char *p = str; *p; *p++, i++) {
        for (int i = 0; i < 6; i++) {
            if (*(p) == control_char[i]) {
                printf("^X");
                i = 6;
            } else if (*(p) == 127) {
                printf("^?");
                i = 6;
            } else if ((*(p) < 0) || (*(p) > 127)) {
                printf("M-");
                i = 6;
            } else if ((*(p) == 9) && (is_t == 1)) {
                printf("^I");
                i = 6;
            } else {
                printf("%c", *(p));
                i = 6;
            }
        }
    }
}

void n_space(int digit, char * space) {
    int count = 0;
    while (digit != 0) {
        digit = digit / 10;
        count++;
    }
    int last_digit = 4;
    if (count > 1) {
        for (int i = count-1; i > 0; i--, last_digit--) {
            space[last_digit] = 0;
        }
    }
}
