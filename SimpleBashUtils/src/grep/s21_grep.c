#include "./s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/array_processing.h"
#include "../common/open_read_file.h"

int main(int argc, char* argv[]) {
  flags is_flags;
  patterns* list = NULL;

  if (argc > 2) {
    if (data_parse(argc, argv, &is_flags, &list)) {
      start_task(argc, argv, &is_flags, list);
    }
    for (patterns* p = list; p != NULL; p = p->next) {
      free(p);
    }
  } else {
    fprintf(stderr, "usage: grep [-ivclnhso] [-e pattern]\n");
  }
  return 0;
}

void struct_init(flags* is_flags) {
  is_flags->e = 0;
  is_flags->i = 0;
  is_flags->v = 0;
  is_flags->c = 0;
  is_flags->l = 0;
  is_flags->n = 0;
  is_flags->h = 0;
  is_flags->s = 0;
  is_flags->f = 0;
  is_flags->o = 0;
}

int data_parse(int argc, char* argv[], flags* is_flags, patterns** list) {
  struct_init(is_flags);
  int result = 0;

  int i = 0;
  int error = 0;
  while ((i = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, &optind)) != -1) {
    if (i == 'e') {
      is_flags->e++;
      push(list, optarg);
    } else if (i == 'i') {
      is_flags->i = 1;
    } else if (i == 'v') {
      is_flags->v = 1;
    } else if (i == 'c') {
      is_flags->c = 1;
    } else if (i == 'l') {
      is_flags->l = 1;
    } else if (i == 'n') {
      is_flags->n = 1;
    } else if (i == 'h') {
      is_flags->h = 1;
    } else if (i == 's') {
      is_flags->s = 1;
    } else if (i == 'o') {
      is_flags->o = 1;
    } else if (i == 'f') {
      is_flags->f = 1;
      flag_f(list, optarg, is_flags);
    } else if (i == '?') {
      fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...\n");
      error = -1;
    }
  }
  if (error == 0) {
    if (is_flags->e == 0 && is_flags->f != 1) {
      push(list, argv[optind]);
      optind++;
      is_flags->e++;
    }
    result = 1;
  }
  return result;
}

void start_task(int argc, char* argv[], flags* is_flags, patterns* list) {
  int cflags = (is_flags->i == 1) ? REG_EXTENDED | REG_ICASE : REG_EXTENDED;
  regex_t preg;
  size_t nmatch = 1;
  regmatch_t pmatch[1];

  for (int file = optind; file < argc; file++) {  // files
    char** str = read_file(argv[file]);
    if (str == NULL) {
      if (is_flags->s != 1) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[file]);
      }
    } else {
      int lines = atoi(str[0]);
      int* indexes_match =
          (int*)malloc((lines * is_flags->e + 1) * sizeof(int));
      int index = 0;

      for (patterns* p = list; p != NULL; p = p->next) {  // templates
        if (regcomp(&preg, p->data, cflags) == 0) {
          for (int string = 1; string < lines; string++) {  // strings
            if (regexec(&preg, str[string], nmatch, &pmatch[0], 0) == 0) {
              indexes_match[index] = string;
              index++;
            }
          }
          regfree(&preg);
        }
      }
      indexes_match[index] = -1;
      if (index != 0) {
        unique(indexes_match, index);
      } else {
        indexes_match[0] = -1;
      }
      output(indexes_match, str, is_flags, argv[file], argc, list, cflags);
      free(indexes_match);
      free(str);
    }
  }
}

void flag_f(patterns** list, char* filename, flags* is_flags) {
  char** str = read_file(filename);
  if (str == NULL) {
    fprintf(stderr, "No SUCH FILE %s", filename);
  } else {
    for (int string = 1; string < atoi(str[0]); string++) {
      int ch = '\n';
      char* ach;
      ach = strchr(str[string], ch);

      if ((ach != NULL) && (str[string][0] != ch)) {
        str[string][ach - str[string]] = 0;
      }
      is_flags->e++;
      push(list, str[string]);
    }
    free(str);
  }
}

void flag_v(int* indexes_match, int lines) {
  int* inverse_indexes = (int*)malloc(lines * sizeof(int));
  int index = 0;

  for (int i = 1; i < lines; i++) {
    int match = 0;
    for (int line = 0; indexes_match[line] != -1; line++) {
      if (i == indexes_match[line]) {
        match++;
      }
    }
    if (match == 0) {
      inverse_indexes[index] = i;
      index++;
    }
  }
  inverse_indexes[index] = -1;
  memset(indexes_match, 0, sizeof(int) * lines);
  memcpy(indexes_match, inverse_indexes, sizeof(int) * (index + 1));
  free(inverse_indexes);
}

void flag_o(patterns* list, char** str, int cflags, flags* is_flags,
            char* filename, int* indexes_match, int argc) {
  for (int s = 0; indexes_match[s] != -1; s++) {
    char* string = str[indexes_match[s]];
    if (str[indexes_match[s]][0] != 0) {
      if (argc - optind > 1 && is_flags->h != 1) {
        printf("%s:", filename);
      }
      if (is_flags->n == 1) {
        printf("%d:", indexes_match[s]);
      }
    }

    for (patterns* p = list; p != NULL; p = p->next) {
      regex_t regex;
      regmatch_t pmatch[1];
      int len;

      regcomp(&regex, p->data, cflags);

      for (int i = 0; i < 5; i++) {
        if (regexec(&regex, string, 1, pmatch, 0)) break;

        len = pmatch[0].rm_eo - pmatch[0].rm_so;
        printf("%.*s\n", len, string + pmatch[0].rm_so);

        string += pmatch[0].rm_eo;
      }
      regfree(&regex);
    }
  }
}

void output(int* indexes_match, char** str, flags* is_flags, char* filename,
            int argc, patterns* list, int cflags) {
  int count_matches = 0;
  for (int i = 0; indexes_match[i] != -1; i++) count_matches++;
  int lines = atoi(str[0]);

  if (is_flags->v == 1) {
    flag_v(indexes_match, atoi(str[0]));
    count_matches = lines - 1 - count_matches;
    if (str[lines - 1][0] == 0) count_matches--;
  }
  count_matches = (is_flags->l == 1 && count_matches > 0) ? 1 : count_matches;
  if (is_flags->c == 1) {
    if (((argc - optind) > 1) && (is_flags->h != 1)) {
      printf("%s:%d\n", filename, count_matches);
    } else {
      printf("%d\n", count_matches);
    }
  }
  if (is_flags->l == 1 && indexes_match[0] != -1) {
    printf("%s\n", filename);
  }
  if (is_flags->l != 1 && is_flags->c != 1) {
    if (is_flags->o == 1 && is_flags->v != 1) {
      flag_o(list, str, cflags, is_flags, filename, indexes_match, argc);
    } else {
      int i = 0;
      for (; indexes_match[i] != -1; i++) {
        if (str[indexes_match[i]][0] != 0) {
          if (argc - optind > 1 && is_flags->h != 1) {
            printf("%s:", filename);
          }
          if (is_flags->n == 1) {
            printf("%d:", indexes_match[i]);
          }
        }
        printf("%s", str[indexes_match[i]]);
      }
      if (indexes_match[0] != -1) {
        if (indexes_match[i - 1] == atoi(str[0]) - 1) {
          if (str[indexes_match[i - 1]][0] != 0) {
            printf("\n");
          }
        }
      }
    }
  }
}

void push(patterns** list, Data* pattern) {
  patterns* p = malloc(sizeof(patterns));
  strcpy(p->data, pattern);
  p->next = *list;
  *list = p;
}
