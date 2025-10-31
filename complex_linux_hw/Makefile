CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS_TASK1 = -L./task1/lib -lcalc -lm
LDFLAGS_TASK2 = -L$(TASK2_DIR) -lconvert

TASK1_DIR = task1
TASK1_LIB_DIR = $(TASK1_DIR)/lib
TASK1_SOURCES = $(wildcard $(TASK1_LIB_DIR)/*.c)
TASK1_OBJECTS = $(TASK1_SOURCES:.c=.o)
TASK1_MAIN = $(TASK1_DIR)/main.c
TASK1_EXECUTABLE = $(TASK1_DIR)/calculator

TASK2_DIR = task2
TASK2_LIB_DIR = $(TASK2_DIR)/lib
TASK2_SOURCES = $(wildcard $(TASK2_LIB_DIR)/*.c)
TASK2_OBJECTS = $(TASK2_SOURCES:.c=.o)
TASK2_MAIN = $(TASK2_DIR)/main.c
TASK2_EXECUTABLE = $(TASK2_DIR)/converter

all: task1 $(TASK2_EXECUTABLE)

# Task 1 
task1: $(TASK1_EXECUTABLE)

$(TASK1_EXECUTABLE): $(TASK1_MAIN) $(TASK1_LIB_DIR)/libcalc.a
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS_TASK1)

$(TASK1_LIB_DIR)/libcalc.a: $(TASK1_OBJECTS)
	ar rcs $@ $^

$(TASK1_LIB_DIR)/%.o: $(TASK1_LIB_DIR)/%.c $(TASK1_LIB_DIR)/calc.h
	$(CC) $(CFLAGS) -c $< -o $@
	
# Task 2
$(TASK2_EXECUTABLE): $(TASK2_MAIN) $(TASK2_DIR)/libconvert.so
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS_TASK2)

$(TASK2_DIR)/libconvert.so: $(TASK2_OBJECTS)
	$(CC) -shared $^ -o $@

$(TASK2_LIB_DIR)/%.o: $(TASK2_LIB_DIR)/%.c $(TASK2_LIB_DIR)/convert.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

install: $(TASK2_DIR)/libconvert.so
	sudo cp $(TASK2_DIR)/libconvert.so /usr/lib
	sudo chmod 0755 /usr/lib/libconvert.so
	sudo ldconfig
	rm -f $(TASK2_DIR)/libconvert.so

uninstall:
	sudo rm -f /usr/lib/libconvert.so
	sudo ldconfig

clean:
	rm -f $(TASK1_DIR)/*.o $(TASK1_LIB_DIR)/*.o $(TASK1_LIB_DIR)/libcalc.a $(TASK1_EXECUTABLE)
	rm -f $(TASK2_DIR)/*.o $(TASK2_LIB_DIR)/*.o $(TASK2_DIR)/libconvert.so $(TASK2_EXECUTABLE)


