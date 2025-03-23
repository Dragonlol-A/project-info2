TARGET = filter

SRC_DIR = src
OBJ_DIR = build

CC = gcc
AR = ar
CFLAGS = --std=c99 --pedantic -Wall -W -Wextra -Wmissing-prototypes -I libpnm -I src
LDFLAGS = -L. -lpnm -lm

SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS) libpnm.a
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

libpnm.a: pnm.o
	$(AR) rcs $@ $^

pnm.o: libpnm/pnm.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC_DIR)/filter.c $(SRC_DIR)/filter_functions.c libpnm.a
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) libpnm.a pnm.o test