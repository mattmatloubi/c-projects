CC=gcc
BASEFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Wvla -Wfloat-equal
PRODFLAGS=$(BASEFLAGS) -O1
DBGFLAGS=$(BASEFLAGS) -fsanitize=address -fsanitize=undefined -g3 
LDLIBS= -lm

# Production Code
huffman : huffman.c output.c driver.c mydefs.h
	$(CC) huffman.c output.c driver.c -o huffman $(PRODFLAGS) $(LDLIBS)

# Development Code
huffman_d : huffman.c output.c driver.c mydefs.h
	$(CC) huffman.c output.c driver.c -o huffman_d $(DBGFLAGS) $(LDLIBS)

all : huffman huffman_d

clean:
	rm -f huffman huffman_d
