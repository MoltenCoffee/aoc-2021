CC := gcc
CFLAGS := -std=c18 -Wall -c -O3 -fPIC
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

.PHONY: all clean

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

.clean:
	rm -f $(OBJS)
