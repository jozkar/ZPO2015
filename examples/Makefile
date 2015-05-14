CFLAGS += -g -std=c99 -Wall -pedantic
GIFCFLAGS += -L../zpogif -I../zpogif/include -lzpogif
PCXCFLAGS += -L../zpopcx -I../zpopcx/include -lzpopcx

all: example1 example2 example3

example1: example1.c
	gcc $(CFLAGS) $(GIFCFLAGS) $< -o $@

example2: example2.c
	gcc $(CFLAGS) $(GIFCFLAGS) $< -o $@

example3: example3.c
	gcc $(CFLAGS) $(PCXCFLAGS) $< -o $@

clean:
	rm -f example1 example2 example3

.PHONY: clean all
