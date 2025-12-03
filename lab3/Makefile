CC=clang
CFLAGS=-std=c17 -Wall -Wextra -O2
OBJS=main.o pager.o parser.o print.o fifo.o lru.o opt.o

memsim: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) memsim
