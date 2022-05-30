CAT_DIR := .
COMMON_DIR := ../common

SRC_CAT = $(shell find $(CAT_DIR) -maxdepth 1 -name '*.c')
OBJ_CAT := $(SRC_CAT:.c=.o)

SRC_COMMON = $(shell find $(COMMON_DIR) -maxdepth 1 -name 's21*.c')
OBJ_COMMON := $(SRC_COMMON:.c=.o)

OS := $(shell uname -s)

CFLAGS := -c -Wall -Wextra -Wall -std=c11

ifeq ($(OS), Linux)
    LFLAGS = -lpcre  # <--- unix
else
    LFLAGS =                            # <--- macOs
endif

CC := gcc

all: s21_grep s21_cat
 
s21_cat: $(OBJ_CAT) $(OBJ_COMMON)
	$(CC) $(OBJ_CAT) $(OBJ_COMMON) $(LFLAGS) -o $(CAT_DIR)/s21_cat

$(OBJ_CAT): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_COMMON): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf ./*.o      \
	       ./s21_cat  \
	       ../common/*.o

.PHONY: all          \
		clean        \
		s21_grep
