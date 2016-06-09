SRCS = mkfs.c

OBJS = $(SRCS:.c=.o)

TARGETS = $(OBJS:.o=)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $<

$(TARGETS): $(OBJS)
	$(CC) -o $@ $<

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS) a.out
