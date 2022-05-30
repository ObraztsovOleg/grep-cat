#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>

#define FLAGS_NUMBER_NONBLANK             (1U <<  0U)
#define FLAGS_E_IMPLIES_V                 (1U <<  1U)
#define FLAGS_NUMBER                      (1U <<  2U)
#define FLAGS_SQUEESE_BLANK               (1U <<  3U)
#define FLAGS_T_IMPLIES_V                 (1U <<  4U)
#define FLAGS_MULTIPLE_LINES              (1U <<  5U)

#define MAX_MSG_LEN                       200

struct option long_options[] = {
    { "number-nonblank", no_argument, NULL, 'b' },
    { "number", no_argument, NULL, 'n' },
    { "squeeze-blank", no_argument, NULL, 's' },
    { NULL, 0, NULL, 0 }
};

void s21_cat(int argc, char * argv[]);

#endif  // SRC_CAT_S21_CAT_H_
