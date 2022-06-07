#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>

#define FLAGS_REGEXP                      (1U <<  0U)
#define FLAGS_IGNORE_CASE                 (1U <<  1U)
#define FLAGS_INVERT_MATCH                (1U <<  2U)
#define FLAGS_COUNT                       (1U <<  3U)
#define FLAGS_FILES_WITH_MATCHES          (1U <<  4U)
#define FLAGS_LINE_NUMBER                 (1U <<  5U)
#define FLAGS_LAST_CHAR                   (1U <<  6U)

#define REGEX_BASIC_CH  '?', '+', '{', '|', '(', ')', '}'
#define REGEX_BASIC_CH_LEN 7

#define HIGHLIGHTER 0

struct option long_options[] = {
    { "regexp", required_argument, NULL, 'e' },
    { "ignore-case", no_argument, NULL, 'i' },
    { "invert-match", no_argument, NULL, 'v' },
    { "count", no_argument, NULL, 'c' },
    { "files-with-matches", no_argument, NULL, 'l' },
    { "line-number", no_argument, NULL, 'n' },
    { NULL, 0, NULL, 0 }
};

void s21_grep(int argc, char * argv[]);

#endif  // SRC_GREP_S21_GREP_H_
