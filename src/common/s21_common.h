#ifndef SRC_COMMON_S21_COMMON_H_
#define SRC_COMMON_S21_COMMON_H_

enum path_type {
    FILE_EXISTS        = 18,
    NOT_IS_DIR_OR_FILE = 2,
    IS_DIR             = 21
};

int what_is(char * path);

FILE * open_file(char * filename);

int close_file(FILE * file);

#endif  // SRC_COMMON_S21_COMMON_H_
