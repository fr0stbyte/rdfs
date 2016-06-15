obj-m := rdfs.o

CFLAGS_rdfs.o := -DDEBUG

rdfs-objs := super.o main.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
