GREP_DIR := .
COMMON_DIR := ../common

SRC_GREP = $(shell find $(GREP_DIR) -maxdepth 1 -name 's21*.c')
OBJ_GREP := $(SRC_GREP:.c=.o)

SRC_COMMON = $(shell find $(COMMON_DIR) -maxdepth 1 -name 's21*.c')
OBJ_COMMON := $(SRC_COMMON:.c=.o)

OS := $(shell uname -s)

CFLAGS := -c -Wall -Wextra -Wall -std=c11

ifeq ($(OS), Linux)
    LFLAGS = -lpcre                    # <--- unix
else
    LFLAGS =                            # <--- macOs
endif

CC := gcc

all: s21_grep

s21_grep: $(OBJ_GREP) $(OBJ_COMMON)
	$(CC) $(OBJ_GREP) $(OBJ_COMMON) $(LFLAGS) -o $(GREP_DIR)/s21_grep

$(OBJ_GREP): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_COMMON): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf ./s21_grep     \
	       ./*.o          \
	       ../common/*.o

.PHONY: all          \
		clean        \
		s21_grep
