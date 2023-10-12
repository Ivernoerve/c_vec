MATRIX_SRC=matrix.c




# Directories
INCLUDE_DIR=include
SRC_DIR=src


TEST_SRC= test.c $(MATRIX_SRC)
TEST_SRC:=$(patsubst %.c,$(SRC_DIR)/%.c, $(TEST_SRC))

LOG_REG_SRC= logistic_regression.c $(MATRIX_SRC)
LOG_REG_SRC:=$(patsubst %.c,$(SRC_DIR)/%.c, $(LOG_REG_SRC))

HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

CFLAGS=-Wall -pg -D_GNU_SOURCE -D_REENTRANT

DEBUG_FLAG = -DDEBUG
.PHONY=all

all: test logreg

test: $(TEST_SRC) $(HEADERS) Makefile
	gcc  -o $@ $(CFLAGS) $(TEST_SRC) -I$(INCLUDE_DIR) -g -lm -lpthread $(DEBUG_FLAG) 

logreg: $(LOG_REG_SRC) $(HEADERS) Makefile
	gcc  -o $@ $(CFLAGS) $(LOG_REG_SRC) -I$(INCLUDE_DIR) -g -lm -lpthread $(DEBUG_FLAG) 

clean:
	rm -f *~ *.o *.exe test logreg && rm -rf *.dSYM  





