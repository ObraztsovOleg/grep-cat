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

void insert_node(struct string_node ** list, int size, char * origin_str) {
    struct string_node * new_node = (struct string_node *) malloc(sizeof(struct string_node));

    new_node->str = (char *) malloc((size + 1) * sizeof(char));
    strcpy(new_node->str, origin_str);

    new_node->next = NULL;

    if (*list == NULL) {
         *list = new_node;
    } else {
        struct string_node * last_node = *list;

        while (last_node->next != NULL) {
            last_node = last_node->next;
        }
        last_node->next = new_node;
    }
}

void write_paths(const char * argv[], struct string_node ** path_names, \
                 char * utility, int * flag_num) {
    char * path = NULL;

    if (argv[(*flag_num)] != NULL) {
        while ((path = (char *) argv[(*flag_num)++]) != NULL) {
            int what = 0;
            if ((what = what_is(path)) == FILE_EXISTS) {
                insert_node(path_names, (int) strlen(path), path);
            } else {
                char error_str[1000] = "\0";
                sprintf(error_str, "%s: %s: %s", utility, path, strerror(what));
                insert_node(path_names, (int) strlen(error_str), error_str);
            }
        }
    }
}

int count_nodes(struct string_node ** list) {
    struct string_node * current = *list;
    struct string_node * next = NULL;
    int num_of_nodes = 0;

    while (current != NULL) {
        next = current->next;
        num_of_nodes++;
        current = next;
    }

    return num_of_nodes;
}

char * pop_list(struct string_node ** list) {
    char * result = NULL;

    if (*list != NULL) {
        struct string_node * head = *list;
        result = head->str;

        (*list) = (*list)->next;
        free(head);
    }

    return result;
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
