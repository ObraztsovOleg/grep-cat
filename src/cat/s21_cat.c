#define  _GNU_SOURCE
#include <stdio.h>
#include "./s21_cat.h"
#include "../common/s21_common.h"

unsigned int flags = 0U;

void _set_flags(int argc, char * argv[]) {
    const char * short_options = "benst";

    int rez;
    int option_index;

    while ((rez = getopt_long(argc, argv, short_options,
        long_options, &option_index)) != -1) {
        switch (rez) {
            case 'b':
                flags |= FLAGS_NUMBER_NONBLANK;
                flags &= ~FLAGS_NUMBER;
                break;
            case 'e':
                flags |= FLAGS_E_IMPLIES_V;
                break;
            case 'n':
                if (!(flags & FLAGS_NUMBER_NONBLANK)) {
                    flags |= FLAGS_NUMBER;
                }
                break;
            case 's':
                flags |= FLAGS_SQUEESE_BLANK;
                break;
            case 't':
                flags |= FLAGS_T_IMPLIES_V;
                break;

            default: {
                printf("found unknown option\n");
                break;
            }
        }
    }
}

void _implie_v(int * ch) {
    if ((*ch >= 0x00 && *ch <= 0x1F) && *ch != 0x0A && *ch != 0x09) {
        printf("%c", '^');
        *ch = *ch + 64;
    }
    if (*ch == 0x7F) {
        printf("%c", '^');
        *ch = '?';
    }

    if (*ch >= 0x80 && *ch <= 0x9F) {
        printf("%s", "M-^");
        *ch = *ch - 64;
    }
}

void _parse_file(FILE * f) {
    long int line_number = 1;
    int ch;
    int prev_symb = '\n';

    while ((ch = getc(f)) != EOF) {
        if ((flags & FLAGS_SQUEESE_BLANK) && prev_symb == '\n') {
            flags &= ~FLAGS_MULTIPLE_LINES;
        }
        if ((flags & FLAGS_SQUEESE_BLANK) && (flags & FLAGS_MULTIPLE_LINES) && prev_symb == '\n') {
            continue;
        }
        if ((flags & FLAGS_SQUEESE_BLANK) && prev_symb == '\n') {
            flags |= FLAGS_MULTIPLE_LINES;
        }

        if ((flags & FLAGS_NUMBER_NONBLANK) && ch != '\n' && prev_symb == '\n') {
            printf("%6ld\t", line_number++);
        } else if (flags & FLAGS_NUMBER && prev_symb == '\n') {
            printf("%6ld\t", line_number++);
        }

        if ((flags & FLAGS_T_IMPLIES_V) && ch == '\t') {
            printf("%c", '^');
            ch = 'I';
        }
        if ((flags & FLAGS_E_IMPLIES_V) || (flags & FLAGS_T_IMPLIES_V)) {
            _implie_v(&ch);
        }
        if ((flags & FLAGS_E_IMPLIES_V) && ch == '\n') {
            printf("%c", '$');
        }

        printf("%c", ch);
        prev_symb = ch;
    }
}

void _open_cat_file(char * path_name) {
    if (path_name == NULL) {
        _parse_file(stdin);
    } else {
        FILE * f = NULL;
        f = fopen(path_name, "r");

        fseek(f, 0L, SEEK_SET);
        _parse_file(f);

        close_file(f);
    }
}

void s21_cat(int argc, char * argv[]) {
    _set_flags(argc, argv);

    struct string_node * path_names = NULL;
    write_paths((const char **) argv, &path_names, "cat", &optind);

    if (path_names == NULL) {
        _open_cat_file(NULL);
    } else {
        char * path_name = NULL;

        while ((path_name = pop_list(&path_names)) != NULL) {
            if (what_is(path_name) == FILE_EXISTS) {
                _open_cat_file(path_name);
            } else {
                fprintf(stderr, "%s\n", path_name);
            }

            free(path_name);
        }
    }
}

int main(int argc, char *argv[]) {
    s21_cat(argc, argv);

    return 0;
}
