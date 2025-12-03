CC = gcc
TARGET = main
SOURCES = main.c server.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = server.h

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)

.PHONY: clean all