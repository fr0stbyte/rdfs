TARGETS = readfs mkfs
CFLAGS += -Wall -g -I. -I./include -I../rdfs
LDFLAGS += -L./lib -lrdfs
all: $(TARGETS)

lib: force_look
	cd lib; $(MAKE) $(MFLAGS) all

readfs: lib
	$(CC) $(CFLAGS) -o readfs readfs.c $(LDFLAGS)

mkfs: lib
	$(CC) $(CFLAGS) -o mkfs mkfs.c $(LDFLAGS)

clean:
	rm -f *.o $(TARGETS) a.out librdfs.a
	cd lib; $(MAKE) clean

force_look:
	true
