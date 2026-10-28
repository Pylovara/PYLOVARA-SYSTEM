# =============================================================================
# Makefile – Pylovara MCS Kernel
# Stand: 08. Januar 2026
# Autor: ID-DNA-THOMAS-ZIMMERMANN-STUFE10-b7F3A
# =============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I./include

# Alle Quellen
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = pylovara_kernel

all: $(TARGET)

$(TARGET): main.c $(OBJECTS)
	$(CC) $(CFLAGS) main.c $(OBJECTS) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
