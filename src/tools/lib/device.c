#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

#include "device.h"

int open_device(const char *dev, int mode) {
  int fd;
  if((fd = open(dev, mode)) < 0) {
    fprintf(stderr, "failed to open device %s\n", dev);
    perror("[open_device]");
    exit(1);
  }
  return fd;
}

void verify_device_space(int fd) {
  if(lseek(fd, (RD_MAXBLOCKS * RD_BSIZE), SEEK_SET) < 0) {
    fprintf(stderr, "device does not have enough space\n");
    perror("[verify_device_space]");
    exit(1);
  }
}
