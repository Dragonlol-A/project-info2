TARGET = filter

SRC_DIR = src
OBJ_DIR = build

CC = gcc
AR = ar
CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes -I libpnm -I src

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ -L libpnm.a -I libpnm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


librairy: pnm.o
	ar rcs libpnm.a pnm.o 

pnm.o: pnm.c
	gcc -c pnm.c -o pnm.o 

test: filter.c filter_functions.c
	gcc filter.c filter_functions.c -L libpnm.a -o test -I .


clean:
	rm -rf $(OBJ_DIR) $(TARGET)