valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file="./cat/cat_log.txt" ./cat/s21_cat -benst ./cat/s21_cat.c
valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file="./grep/grep_log.txt" ./grep/s21_grep -nivl 21 ./cat/s21_cat.c