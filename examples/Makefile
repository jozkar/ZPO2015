CFLAGS += -g -std=c99 -Wall -pedantic
GIFCFLAGS += -L../zpogif -I../zpogif/include -lzpogif

all: example1 example2

example1: example1.c
	gcc $(CFLAGS) $(GIFCFLAGS) $< -o $@

example2: example2.c
	gcc $(CFLAGS) $(GIFCFLAGS) $< -o $@

clean:
	rm -f example1 example2

.PHONY: clean all
