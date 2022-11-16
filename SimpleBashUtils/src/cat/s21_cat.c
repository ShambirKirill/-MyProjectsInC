#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/array_processing.h"
#include "../common/open_read_file.h"

int data_processing(int argc, char* argv[], int* flags, int* indexes_files);
int define_option(char* arg);
int validation_flags(int* flags);
void start_task(int* flags, char** str);
void s21_cat_s(char* str, int* mark_empty);
void s21_cat_b(char* str, int* line_num);
void s21_cat_e(char* str);
void s21_cat_n(char* str, int* line_num);
void s21_cat_t(char* str);
void non_printing(char* str);
void output(char** str);

int main(int argc, char* argv[]) {
  int flags[12];
  int* indexes_files = calloc(argc, sizeof(int));
  if (indexes_files == NULL) {
    printf("calloc error. Terminated");
  } else {
    if (data_processing(argc, argv, flags, indexes_files)) {
      if (validation_flags(flags)) {
        for (int i = 0; indexes_files[i] != -1; i++) {
          char** str = read_file(argv[indexes_files[i]]);
          if (str != NULL) {
            start_task(flags, str);
            output(str);
            free(str);
          } else {
            fprintf(stderr, "s21_cat: %s: No such file or directory\n",
                    argv[indexes_files[i]]);
          }
        }
      }
    }
  }
  free(indexes_files);
  return 0;
}

int data_processing(int argc, char* argv[], int* flags, int* indexes_files) {
  int index_flag =
      0;  // determines which index to assign to the flags of the variable
  int index_file =
      0;  // determines which index to assign to the files of the variable
  for (int i = 1; i < argc; i++) {
    int option = define_option(argv[i]);
    if (option) {
      int exists = 0;
      for (int el = 0; el < index_flag; el++) {
        if (option == flags[el]) {
          exists = 1;
          break;
        }
      }
      if (!exists) {
        flags[index_flag] = option;
        index_flag++;
      }
    } else {
      indexes_files[index_file] = i;
      index_file++;
    }
  }
  flags[index_flag] = -1;
  indexes_files[index_file] = -1;
  return ((index_flag > 0) || (index_file > 0)) ? 1 : 0;
}

int define_option(char* arg) {
  // The function determines which flag the entered key belongs to
  int result = 0;
  char flags[10][20] = {"-s",       "-t",
                        "-b",       "-n",
                        "-e",       "--squeeze-blank",
                        "-T",       "--number-nonblank",
                        "--number", "-E"};
  for (int i = 0; i < 10; i++) {
    if (strcmp(arg, flags[i]) == 0) {
      result = i + 1;
      break;
    }
  }
  return result;
}

int validation_flags(int* flags) {
  /* Check for a combination of gnu flags and non-gnu flags.
  Combining them is forbidden*/
  int not_gnu = 0;
  int is_gnu = 0;
  int result = 1;
  int i = 0;
  for (; flags[i] != -1; i++) {
    if (flags[i] > 5) {
      is_gnu = 1;
    } else if (flags[i] < 6) {
      not_gnu = 1;
    }
  }

  if ((not_gnu == 1) && (is_gnu == 1)) {
    result = 0;
  } else {  // Sorting is needed for prioritization
    sort_array(flags, i);
  }
  return result;
}

void start_task(int* flags, char** str) {
  int line_num = 0;
  int mark_empty = 0;
  int mark_b =
      0;  // -b is in priority, so if it was running, -n will no longer run

  for (int i = 0; flags[i] != -1; i++) {
    for (int k = 1; k < atoi(str[0]); k++) {
      if ((flags[i] == 1) || (flags[i] == 6)) {
        s21_cat_s(str[k], &mark_empty);
      } else if ((flags[i] == 2) || (flags[i] == 7)) {
        if (flags[i] == 2) {
          non_printing(str[k]);
        }
        s21_cat_t(str[k]);
      } else if ((flags[i] == 3) || (flags[i] == 8)) {
        mark_b = 1;
        s21_cat_b(str[k], &line_num);
      } else if (((flags[i] == 4) || (flags[i] == 9)) && (mark_b == 0)) {
        s21_cat_n(str[k], &line_num);
      } else if ((flags[i] == 5) || (flags[i] == 10)) {
        if (flags[i] == 5) {
          non_printing(str[k]);
        }
        s21_cat_e(str[k]);
      }
    }
  }
}

void s21_cat_s(char* str, int* mark_empty) {
  if (strlen(str) != 1) {
    *mark_empty = 0;
  } else if (str[0] == 10) {
    if (*mark_empty == 0) {
      *mark_empty = 1;
    } else {
      str[0] = '\0';
    }
  }
}

void s21_cat_b(char* str, int* line_num) {
  if (str[0] != 10 && (str[0] != 0)) {
    *(line_num) = *(line_num) + 1;
    char string[1024];
    sprintf(string, "%6d\t%s", *(line_num), str);
    strcpy(str, string);
  }
}

void s21_cat_e(char* str) {
  char* ach = strrchr(str, '\n');
  int last_char_pos = ach - str + 1;
  if (ach != NULL) {
    str[last_char_pos - 1] = '$';
    str[last_char_pos] = '\n';
  }
}

void s21_cat_n(char* str, int* line_num) {
  if (str[0] != 0) {
    char string[1024];
    *(line_num) = *(line_num) + 1;
    sprintf(string, "%6d\t%s", *(line_num), str);
    strcpy(str, string);
  }
}

void s21_cat_t(char* str) {
  if (str[0] != 0) {
    char string[1024];
    int index_string = 0;
    for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] == 9) {
        sprintf(&string[index_string], "^%c", str[i] + 64);
        index_string += 2;
      } else {
        string[index_string] = str[i];
        index_string++;
      }
    }
    string[index_string] = '\0';
    strcpy(str, string);
  }
}

void non_printing(char* str) {
  // The function replaces non-printable characters
  if (str[0] != 0) {
    char string[1024];
    int index_string = 0;
    int i = 0;

    while (1) {
      if (str[i] < -96) {
        sprintf(&string[index_string], "M-^%c", str[i] + 192);
        index_string += 4;
      } else if (str[i] < 0) {
        sprintf(&string[index_string], "M-%c", str[i] + 128);
        index_string += 3;
      } else if (str[i] < 32) {
        if (str[i] == 9 || str[i] == 10) {
          sprintf(&string[index_string], "%c", str[i]);
          index_string++;
        } else {
          if (str[i] == 0) {
            if (str[i + 1] == 0) {
              break;
            }
          }
          sprintf(&string[index_string], "^%c", str[i] + 64);
          index_string += 2;
        }
      } else if (str[i] < 127) {
        sprintf(&string[index_string], "%c", str[i]);
        index_string++;
      } else {
        sprintf(&string[index_string], "^?");
        index_string += 2;
      }
      i++;
    }
    string[index_string] = '\0';
    strcpy(str, string);
  }
}

void output(char** str) {
  for (int i = 1; i < atoi(str[0]); i++) {
    printf("%s", str[i]);
  }
}
