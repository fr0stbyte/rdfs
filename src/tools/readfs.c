#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#include "readfs.h"
#include "../module/include/rdfs.h"

int main(int argc, char ** argv) {
  int devfd;

  if(argc != 2) {
    fprintf(stderr, "need to specify a device\n");
    exit(1);
  }

  devfd = open_device(argv[1]);
  struct rdfs_superblock* sb = read_superblock(devfd);
  print_superblock(sb);
  free(sb);

  return 0;
}

int open_device(char *dev) {
  int fd;
  if((fd = open(dev, O_RDONLY)) < 0) {
    fprintf(stderr, "failed to open device %s\n", dev);
    perror("[open_device]");
    exit(1);
  }
  return fd;
}

struct rdfs_superblock * read_superblock(int fd) {
  struct rdfs_superblock * sb = calloc(1, sizeof(struct rdfs_superblock));
  if(read(fd, (void*)sb, sizeof(sb)) < 0) {
    fprintf(stderr, "failed to read superblock");
    perror("[read_superblock]");
    exit(1);
  }
  return sb;
}

void print_superblock(struct rdfs_superblock* sb) {
  if(NULL == sb) {
    fprintf(stderr, "null superblock given\n");
    exit(1);
  }

  fprintf(stdout, "superblock: \n");
  fprintf(stdout, "\tmagic: %x\n", sb->s_magic);
  fprintf(stdout, "\tmod: %d\n", sb->s_mod);
}
