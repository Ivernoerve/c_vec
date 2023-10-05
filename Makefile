MATRIX_SRC=matrix.c




# Directories
INCLUDE_DIR=include
SRC_DIR=src


TEST_SRC= test.c common.c $(MATRIX_SRC)

TEST_SRC:=$(patsubst %.c,$(SRC_DIR)/%.c, $(TEST_SRC))

HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

CFLAGS=-Wall -pg -D_GNU_SOURCE -D_REENTRANT

DEBUG_FLAG = -DDEBUG
.PHONY=all

all: test

test: $(TEST_SRC) $(HEADERS) Makefile
	gcc  -o $@ $(CFLAGS) $(TEST_SRC) -I$(INCLUDE_DIR) -g -lm -lpthread $(DEBUG_FLAG) 

clean:
	rm -f *~ *.o *.exe test && rm -rf *.dSYM  





