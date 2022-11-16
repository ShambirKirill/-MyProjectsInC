typedef struct flags flags;

typedef int flag;
typedef struct patterns patterns;
typedef char Data;

struct patterns {
  Data data[100];
  patterns* next;
};

struct flags {
  flag e;
  flag i;
  flag v;
  flag c;
  flag l;
  flag n;
  flag h;
  flag s;
  flag o;
  flag f;
};

void struct_init(flags* is_flags);
int data_parse(int argc, char* argv[], flags* is_flags, patterns** list);
void start_task(int argc, char* argv[], flags* is_flags, patterns* list);
void flag_v(int* indexes_match, int lines);
void flag_o(patterns* list, char** str, int cflags, flags* is_flags,
            char* filename, int* indexes_match, int argc);
void flag_f(patterns** list, char* filename, flags* is_flags);
void output(int* indexes_match, char** str, flags* is_flags, char* filename,
            int argc, patterns* list, int cflags);
void push(patterns** list, Data* pattern);
