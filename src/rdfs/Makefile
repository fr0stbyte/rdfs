obj-m := rd.o

CFLAGS_rd.o := -DDEBUG

rd-objs := super.o rdfs.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
