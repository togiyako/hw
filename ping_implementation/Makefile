CC = gcc
TARGET = ping
SRC = ping.c

all:$(TARGET)
	
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean