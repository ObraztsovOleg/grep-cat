#ifndef SRC_COMMON_S21_COMMON_H_
#define SRC_COMMON_S21_COMMON_H_

#include <string.h>
#include <stdlib.h>

enum path_type {
    FILE_EXISTS        = 18,
    NOT_IS_DIR_OR_FILE = 2,
    IS_DIR             = 21
};

enum control {
    PRINT_FILE         = 0,
    DO_NOT_PRINTF_FILE = 1
};

struct string_node {
    char * str;
    struct string_node * next;
};

void write_paths(const char * argv[], struct string_node ** path_names, \
                 char * utility, int * flag_num);

int count_nodes(struct string_node ** list);

char * pop_list(struct string_node ** list);

int what_is(char * path);

FILE * open_file(char * filename);

int close_file(FILE * file);

void insert_node(struct string_node ** list, int size, char * origin_str);

#endif  // SRC_COMMON_S21_COMMON_H_
