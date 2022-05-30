#include <sys/stat.h>
#include <stdio.h>
#include "./s21_common.h"

int what_is(char * path) {
    struct stat stat_buf;
    int ret = NOT_IS_DIR_OR_FILE;

    if (stat(path, &stat_buf) == 0) {
        if (S_ISDIR(stat_buf.st_mode)) {
            ret = IS_DIR;
        } else {
            ret = FILE_EXISTS;
        }
    }

    return ret;
}

FILE * open_file(char * filename) {
    FILE * f;
    f = fopen(filename, "r");

    return f;
}

int close_file(FILE * file) {
    int ret = 0;

    if (file != NULL) {
        fclose(file);
        ret = 1;
    }

    return ret;
}
