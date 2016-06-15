SRCS = device.c superblock.c inode.c direntry.c

OBJS = $(SRCS:%.c=%.o)

CFLAGS = -Wall -I../include -I../../rdfs

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

librdfs.a: $(OBJS)
	$(AR) crs $@ $^

all: librdfs.a

clean:
	rm -f *.o librdfs.a
