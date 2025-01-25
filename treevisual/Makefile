CC=gcc
BASEFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Wvla -Wfloat-equal
PRODFLAGS=$(BASEFLAGS) -O1
DBGFLAGS=$(BASEFLAGS) -fsanitize=address -fsanitize=undefined -g3 
LDLIBS= -lm

# Production Code
visual : visual.c main.c output.c visual.h
	$(CC) visual.c main.c output.c -o visual $(PRODFLAGS) $(LDLIBS)

# Development Code
visual_d : visual.c output.c main.c visual.h
	$(CC) visual.c main.c output.c -o visual_d $(DBGFLAGS) $(LDLIBS)

all : visual visual_d

clean:
	rm -f visual visual_d
