CC = gcc
TARGET = main
SOURCES = src/main.c src/server.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = src/server.h

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $<

clean:
	rm -f src/*.o $(TARGET)

.PHONY: clean all