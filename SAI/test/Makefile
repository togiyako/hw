CC = gcc

STUB_DIR = ../stub
STUB_INC = $(STUB_DIR)/inc
STUB_LIB = $(STUB_DIR)/src/.libs

CFLAGS = -I. -I$(STUB_INC) -I/usr/include/sai
LDFLAGS = -L$(STUB_LIB) -Wl,-rpath,$(abspath $(STUB_LIB)) -L/usr/lib

LDLIBS = -lsai

SRC = test.c
TARGET = bin/unit_test

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) -o $(TARGET) $(CFLAGS) $(SRC) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean