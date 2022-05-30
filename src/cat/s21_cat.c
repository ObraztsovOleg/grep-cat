#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./s21_cat.h"
#include "../common/s21_common.h"

unsigned int flags = 0U;
static char msg[MAX_MSG_LEN];
unsigned int additional_len = 0;

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
                additional_len += 10 * sizeof(char) + 8 * sizeof(char);  // unsigned long int 2^32 - 1
                break;
            case 'e':
                flags |= FLAGS_E_IMPLIES_V;
                additional_len += 1 * sizeof(char);
                break;
            case 'n':
                if (!(flags & FLAGS_NUMBER_NONBLANK)) {
                    additional_len += 10 * sizeof(char) + 8 * sizeof(char);  // unsigned long int 2^32 - 1
                    flags |= FLAGS_NUMBER;
                }
                break;
            case 's':
                flags |= FLAGS_SQUEESE_BLANK; break;
            case 't':
                flags |= FLAGS_T_IMPLIES_V;
                additional_len += 1 * sizeof(char);
                break;
            default: {
                printf("found unknown option\n");
                break;
            }
        }
    }
}

void _put_number(char * buff, long int * line_number, long int * count) {
    char num[sizeof(long int) + 8 * sizeof(char)];
    sprintf(num, "%6ld\t", (*line_number)++);
    strcat(buff, num);
    (*count) += (int) strlen(num);
}

void _implie_v(char * ch, long int * count, char * line_buffer) {
    if ((*ch >= 0x00 && *ch <= 0x1F) && *ch != 0x0A && *ch != 0x09) {
        line_buffer[(*count)++] = '^';
        *ch = *ch + 64;
    }
    if (*ch == 0x7F) {
        line_buffer[(*count)++] = '^';
        *ch = '?';
    }
    if (*ch & 0xFFFFFF00) {
        int ch_int = (int) *ch;
        ch_int &= 0x000000FF;

        if (ch_int >= 0x80 && ch_int <= 0x9F) {
            line_buffer[(*count)++] = 'M';
            line_buffer[(*count)++] = '-';
            line_buffer[(*count)++] = '^';
            *ch = *ch - 64;
        }
    }
}

long int _get_str_len(FILE * f, int ptr_start_pos, int ptr_stop_pos) {
    long int count = 0;
    char ch;

    fseek(f, ptr_start_pos, SEEK_SET);

    while ((ch = getc(f)) != EOF) {
        count++;
        if (ch == '\n') break;
    }

    fseek(f, ptr_stop_pos, SEEK_SET);

    return count;
}

void _parse_file(FILE * f) {
    long int line_number = 1;
    char * str = NULL;
    size_t len = 0;

    while (getline(&str, &len, f) != -1) {
        long int count = 0;
        int ptr_start_pos = 0;
        int ptr_stop_pos = 0;

        ptr_stop_pos = ftell(f);

        long int line_len = _get_str_len(f, ptr_start_pos, ptr_stop_pos);
        if ((flags & FLAGS_E_IMPLIES_V) || (flags & FLAGS_T_IMPLIES_V))
            additional_len += 3 * line_len;

        ptr_start_pos = ftell(f);
        char * line_buffer = (char *) calloc((additional_len + line_len + 1), sizeof(char));

        if ((flags & FLAGS_SQUEESE_BLANK) && line_len > 1) {
            flags &= ~FLAGS_MULTIPLE_LINES;
        }
        if ((flags & FLAGS_SQUEESE_BLANK) && (flags & FLAGS_MULTIPLE_LINES)) {
            if (line_buffer != NULL) {
                free(line_buffer);
            }
            continue;
        }
        if ((flags & FLAGS_SQUEESE_BLANK) && line_len == 1) {
            flags |= FLAGS_MULTIPLE_LINES;
        }

        if ((flags & FLAGS_NUMBER_NONBLANK) && line_len > 1) {
            _put_number(line_buffer, &line_number, &count);
        } else if (flags & FLAGS_NUMBER) {
            _put_number(line_buffer, &line_number, &count);
        }

        for (int i = 0; i < line_len; i++) {
            char ch = str[i];

            if ((flags & FLAGS_T_IMPLIES_V) && ch == '\t') {
                line_buffer[count++] = '^';
                ch = 'I';
            }
            if ((flags & FLAGS_E_IMPLIES_V) || (flags & FLAGS_T_IMPLIES_V)) {
                _implie_v(&ch, &count, line_buffer);
            }
            if ((flags & FLAGS_E_IMPLIES_V) && ch == '\n') {
                line_buffer[count++] = '$';
            }

            line_buffer[count++] = ch;
        }

        for (int i = 0; i < count; i++) {
            printf("%c", line_buffer[i]);
        }

        if (line_buffer != NULL) {
            free(line_buffer);
        }
    }

    free(str);
}

void _open_cat_file(int argc, char * argv[], char * path_name) {
    FILE * f = NULL;

    _set_flags(argc, argv);
    f = open_file(path_name);

    fseek(f, 0L, SEEK_SET);
    _parse_file(f);

    close_file(f);
}

void s21_cat(int argc, char * argv[]) {
    char * path_name = argv[argc - 1];
    int what_amI = what_is(path_name);

    if (what_amI == FILE_EXISTS) {
        _open_cat_file(argc, argv, path_name);
    } else {
        sprintf(msg, "cat: %s: %s", path_name, \
                strerror(what_amI));
        printf("%s\n", msg);
    }
}

int main(int argc, char *argv[]) {
    s21_cat(argc, argv);






    return 0;
}
