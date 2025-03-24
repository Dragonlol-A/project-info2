### 
## Makefile skeleton
## INFO0030: Projet 1
## 
### 

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes
LD=gcc
LDFLAGS=

# Files
EXEC=pnm
MODULES=main.c pnm.c
OBJECTS=main.o pnm.o

## Rules

all: $(EXEC)

pnm: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

pnm.o: pnm.c
	$(CC) -c pnm.c -o pnm.o $(CFLAGS)

clean:
	rm -f *.o $(EXEC) *~


test: $(EXEC)
	./$(EXEC) -i test_image/test.pbm -o test_image/result.pbm
	./$(EXEC) -i test_image/test.pgm -o test_image/result.pgm
	./$(EXEC) -i test_image/test.ppm -o test_image/result.ppm
