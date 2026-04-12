CC      ?= gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2
AR      := ar

all: libcompass.a test_compass

libcompass.a: compass.o
	$(AR) rcs $@ $^

compass.o: compass.c compass.h
	$(CC) $(CFLAGS) -c -o $@ compass.c

test_compass: test_compass.c compass.c compass.h
	$(CC) $(CFLAGS) -o $@ test_compass.c compass.c -lm

test: test_compass
	./test_compass

clean:
	rm -f *.o *.a test_compass

.PHONY: all test clean
