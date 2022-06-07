#define  _GNU_SOURCE
#include <stdio.h>
#include <pcre.h>
#include "./s21_grep.h"
#include "../common/s21_common.h"
#include <stdlib.h>

unsigned int flags = 0U;
char * regex = NULL;

void _set_flags(int argc, char * argv[]) {
    const char * short_options = "e:ivcln";

    int rez;
    int option_index;

    while ((rez = getopt_long(argc, argv, short_options,
        long_options, &option_index)) != -1) {
        switch (rez) {
            case 'e':
                if (optarg != NULL) {
                    flags |= FLAGS_REGEXP;
                    regex = optarg;
                } else {
                    printf("found size without value\n");
                }
            break;
            case 'i':
                flags |= FLAGS_IGNORE_CASE;
            break;
            case 'v':
                flags |= FLAGS_INVERT_MATCH;
            break;
            case 'c':
                flags |= FLAGS_LAST_CHAR;
                flags |= FLAGS_COUNT;
            break;
            case 'l':
                flags |= FLAGS_LAST_CHAR;
                flags |= FLAGS_FILES_WITH_MATCHES;
            break;
            case 'n':
                flags |= FLAGS_LINE_NUMBER;
            break;

            default: {
                printf("found unknown option\n");
                break;
            }
        }
    }
}

pcre * _regex_init(void) {
    const char * error = NULL;
    int erroffset = 0;
    pcre * re = NULL;

    re = pcre_compile(regex,
                      PCRE_MULTILINE,
                      &error,
                      &erroffset,
                      NULL);
    if (!re) {
        printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
    }

    return re;
}

char * _parse_regex(void) {
    char * parsed_re = (char *) calloc(((int) strlen(regex) + 1), sizeof(char));
    const char symbs[REGEX_BASIC_CH_LEN] = {REGEX_BASIC_CH};
    int count = 0;
    int re_length = (int) strlen(regex);

    for (int i = 0; i < re_length; i++) {
        if (regex[i] == '\\') {
            for (int j = 0; j < REGEX_BASIC_CH_LEN; j++) {
                if (regex[i + 1] == symbs[j]) {
                    parsed_re[count++] = symbs[j];
                    i += 2;
                    break;
                }
            }
        }
        parsed_re[count++] = regex[i];
    }

    return parsed_re;
}

void _delete_list(struct string_node ** list) {
    while (*list != NULL) {
        struct string_node * head = *list;
        (*list) = (*list)->next;

        free(head->str);
        free(head);
    }
}

void _ignore_case_convertation(char * reference_str, int len) {
    for (int i = 0; i < (int) strlen(regex); i++) {
        if (regex[i] >= 'A' && regex[i] <= 'Z') {
            regex[i] += 32;
        }
    }
    for (int i = 0; i < (int) len; i++) {
        if (reference_str[i] >= 'A' && reference_str[i] <= 'Z') {
            reference_str[i] += 32;
        }
    }
}

void _find_match(char * str, struct string_node ** list) {
    unsigned int offset = 0;
    unsigned int len = strlen(str);
    int ovector[2] = {0};
    int rc = 0;

    char * reference_str = (char *) calloc(len + 1, sizeof(char));
    (void) strcpy(reference_str, str);

    if (flags & FLAGS_IGNORE_CASE)
        _ignore_case_convertation(reference_str, len);

    pcre * re = _regex_init();

    while (offset < len && (rc = pcre_exec(re, 0, reference_str, len, offset, \
                            0, ovector, sizeof(ovector))) >= 0) {
        char * substring_start = str + ovector[0];
        int substring_length = ovector[1] - ovector[0];
        char * new_str = (char *) calloc(substring_length + 1, sizeof(char));

        memset(new_str, 0, substring_length);
        strncpy(new_str, substring_start, substring_length);

        insert_node(list, strlen(new_str), new_str);

        offset = ovector[1];
        free(new_str);
    }
    if (reference_str != NULL)
        free(reference_str);

    pcre_free(re);
}

void _return_lines_count(long int lines_count, \
                         int control, char * path) {
    if (control == PRINT_FILE) {
        #if HIGHLIGHTER
            printf("\033[0;35m%s\033[0m:", path);
        #else
            printf("%s:", path);
        #endif
    }

    printf("%ld\n", lines_count);
}

void _return_file_name(char * path) {
    #if HIGHLIGHTER
        printf("\033[0;35m%s\033[0m\n", path);
    #else
        printf("%s\n", path);
    #endif
}

void _return_text(char * string, struct string_node ** list, \
                  int control, char * path, int line_number) {
    char * match_str = NULL;
    char * ret = NULL;

    if (control == PRINT_FILE) {
        #if HIGHLIGHTER
            printf("\033[0;35m%s\033[0m:", path);
        #else
            printf("%s:", path);
        #endif
    }

    if (flags & FLAGS_LINE_NUMBER) {
        #if HIGHLIGHTER
            printf("\033[0;32m%d\033[0m:", line_number);
        #else
            printf("%d:", line_number);
        #endif
    }

    while ((match_str = pop_list(list)) != NULL) {
        ret = strstr(string, match_str);

        for (int i = 0; i < (ret - string); i++) {
            printf("%c", string[i]);
        }
        for (int i = 0; i < (int) strlen(match_str); i++) {
            #if HIGHLIGHTER
                printf("\033[1m\033[31m%c\033[0m", match_str[i]);
            #else
                printf("%c", match_str[i]);
            #endif
        }

        string = string + strlen(match_str) + (ret - string);
        free(match_str);
    }

    for (int i = 0; i < (int) strlen(string); i++) {
        printf("%c", string[i]);
    }

    if (string[(int) strlen(string) - 1] != '\n')
        flags |= FLAGS_LAST_CHAR;
    else
        flags &= ~FLAGS_LAST_CHAR;
}

void _grep_stream(FILE * stream, int control, char * path) {
    struct string_node * char_list = NULL;
    char * stdin_str = NULL;
    size_t len = 0;
    long int lines_count = 0;
    long int line_number = 0;

    while (getline(&stdin_str, &len, stream) != -1) {
        line_number++;
        _find_match(stdin_str, &char_list);

        if ((flags & FLAGS_FILES_WITH_MATCHES)) {
            if (char_list != NULL) {
                lines_count++;
                _delete_list(&char_list);
                break;
            } else {
                continue;
            }
        }

        if (char_list == NULL && (flags & FLAGS_INVERT_MATCH)) {
            if (flags & FLAGS_COUNT)
                lines_count++;
            else
                _return_text(stdin_str, &char_list, \
                             control, path, line_number);
        } else if (char_list != NULL && !(flags & FLAGS_INVERT_MATCH)) {
            if (flags & FLAGS_COUNT)
                lines_count++;
            else
                _return_text(stdin_str, &char_list, \
                             control, path, line_number);
        }


        _delete_list(&char_list);
    }

    if ((flags & FLAGS_COUNT)) {
        flags &= ~FLAGS_LAST_CHAR;
        _return_lines_count(lines_count, control, path);
    }

    if ((flags & FLAGS_FILES_WITH_MATCHES)) {
        flags &= ~FLAGS_LAST_CHAR;
        _return_file_name(path);
    }

    free(stdin_str);
}

void s21_grep(int argc, char * argv[]) {
    _set_flags(argc, argv);
    if (regex == NULL && !(flags & FLAGS_REGEXP))
        regex = argv[optind++];

    regex = _parse_regex();

    struct string_node * path_names = NULL;
    write_paths((const char **) argv, &path_names, "grep", &optind);

    if (path_names == NULL) {
        _grep_stream(stdin, DO_NOT_PRINTF_FILE, NULL);
    } else {
        char * path_name = NULL;
        int num_nodes = count_nodes(&path_names);

        while ((path_name = pop_list(&path_names)) != NULL) {
            if (what_is(path_name) == FILE_EXISTS) {
                FILE * stream = open_file(path_name);

                _grep_stream(stream, \
                            (num_nodes > 1) ? PRINT_FILE : DO_NOT_PRINTF_FILE, \
                            path_name);
                fclose(stream);
            } else {
                flags &= ~FLAGS_LAST_CHAR;
                fprintf(stderr, "%s\n", path_name);
            }

            free(path_name);
        }

        if (flags & FLAGS_LAST_CHAR)
            printf("\n");
    }

    free(regex);
}

int main(int argc, char * argv[]) {
    s21_grep(argc, argv);
    return 0;
}
