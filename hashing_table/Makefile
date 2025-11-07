CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude

TARGET = main

VPATH = src
OBJECTS = main.o hash_table.o
DEPS = include/hash_table.h include/status.h

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

	rm -f $(OBJECTS)

clean:
	rm -f $(TARGET)