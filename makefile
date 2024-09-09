CC = gcc
CFLAGS = -Wall -ggdb

EXECUTABLE = strands.exe
MAIN = main.c

# Includes the file containing SOURCES and HEADERS
include defs.mk

$(EXECUTABLE): $(MAIN) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(MAIN) $(SOURCES)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)