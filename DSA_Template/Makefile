CC = gcc
CFLAGS = -g -Wall -Wextra
INCLUDES = -Iinclude -Itests

ifeq ($(DEBUG), y)
	CFLAGS += -DDSA_PERF_MEASURE
endif

TARGET = run_tests

SRCS = main.c $(wildcard src/*.c) $(wildcard tests/src/*.c)

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean