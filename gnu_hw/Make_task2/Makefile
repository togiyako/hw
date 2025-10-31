CC = gcc
TARGET = program
OBJECTS = main.o src1.o src2.o src3.o


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	
main.o : main.c src.h
	$(CC) -c -o $@ $<

ifeq ($(VAR), include)
include make1.mk
include make2.mk
include make3.mk
	
else
	$(MAKE) -f make1.mk src1.o
	$(MAKE) -f make2.mk src2.o
	$(MAKE) -f make3.mk src3.o
endif

clean:
	rm -f $(OBJECTS) $(TARGET)
	
.PHONY: all clean
