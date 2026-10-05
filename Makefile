CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = ls

SOURCES = src/main.c \
          src/options.c \
          src/fileinfo.c \
          src/sort.c \
          src/display.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
