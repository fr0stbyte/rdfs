#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#include "mkfs.h"
#include "../module/include/rdfs.h"

int main(int argc, char **argv) {
  int devfd;

  // get the device from argv
  if(argc != 2) {
    fprintf(stderr, "need to pass in a device\n");
    exit(1);
  }
  // open device
  devfd = open_device(argv[1]);

  // find if there is enough space -- seek blocks * block size
  verify_device_space(devfd);

  // write the superblock
  write_superblock(devfd);

  // setup the inodes for / and lost+found
  // write each inode
  // skip into file section
  // write directories for ., .. and lost+found
  // write dirs

  return 0;
}

int open_device(char* device) {
  int fd = open(device, O_WRONLY);
  if(fd < 0) {
    fprintf(stderr, "failed to open device %s\n", device);
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

void write_superblock(int fd) {
  struct rdfs_superblock sb;
  int i = 0;

  sb.s_mod = RD_FSCLEAN;
  sb.s_magic = RD_MAGIC;
  sb.s_nifree = RD_MAXFILES;
  sb.s_nbfree = RD_MAXBLOCKS;

  fprintf(stdout, "inodes free : %d\n", sb.s_nifree);
  fprintf(stdout, "block free : %d\n", sb.s_nbfree);
  // reserve the first 4 inodes
  for(i = 0; i < 4; i++) {
    sb.s_inodes[i] = RD_INODE_INUSE;
  }

  //set the rest of inodes free
  for(i = 4; i < RD_MAXFILES; i++) {
    sb.s_inodes[i] = RD_INODE_FREE;
  }

  // write at offset 0
  if(pwrite(fd, (void*)&sb, sizeof(sb), 0) < 0) {
    fprintf(stderr, "failed to write superblock\n");
    perror("[write_superblock]");
    exit(1);
  }
}
