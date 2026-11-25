# James Sampson - CS 5343 - Programming 4 (Program 1)
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -O2 -pthread
TARGET = readers_writers

all: $(TARGET)

$(TARGET): readers_writers.c
	$(CC) $(CFLAGS) -o $(TARGET) readers_writers.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
