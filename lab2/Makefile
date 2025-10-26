CC=clang
CFLAGS=-std=c17 -Wall -Wextra -O2

OBJS=main.o sim.o queue.o parser.o heap.o \
     algFCFS.o algRR.o algSRTF.o metrics.o printer.o

myscheduler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) myscheduler
