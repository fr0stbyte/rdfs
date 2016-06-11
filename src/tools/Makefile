TARGETS = readfs mkfs
CFLAGS += -Wall -g -I.
LDFLAGS += -L. -lrdfs 
all: $(TARGETS)

lib:
	$(CC) $(CFLAGS) -c lib.c
	$(AR) -cvq librdfs.a lib.o

readfs: lib 
	$(CC) $(CFLAGS) -o readfs readfs.c $(LDFLAGS)

mkfs: lib
	$(CC) $(CFLAGS) -o mkfs mkfs.c $(LDFLAGS)

clean:
	rm -f *.o $(TARGETS) a.out librdfs.a
