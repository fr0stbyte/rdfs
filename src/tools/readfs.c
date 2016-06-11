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

  loop(devfd);

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

void read_superblock(int fd, struct rdfs_superblock* sb) {
  if(pread(fd, (struct rdfs_superblock*)sb, sizeof(struct rdfs_superblock), 0) < 0) {
    fprintf(stderr, "failed to read superblock");
    perror("[read_superblock]");
    exit(1);
  }
}

void print_superblock(struct rdfs_superblock sb) {
  int i = 0;

  if(sb.s_magic != RD_MAGIC) {
    fprintf(stderr, "this is not a rdfs filesystem. Got %z, expected %z\n", sb.s_magic, RD_MAGIC);
    exit(1);
  }
  fprintf(stdout, "superblock: \n");
  fprintf(stdout, "\tmagic: 0x%x\n", sb.s_magic);
  fprintf(stdout, "\tmod: %s\n", (sb.s_mod == RD_FSCLEAN ? "RD_FSCLEAN": "RD_FSDIRTY"));
  fprintf(stdout, "\tfree blocks: %lu\n", sb.s_nbfree);
  fprintf(stdout, "\tfree inodes: %lu\n", sb.s_nifree);

  for(i = 0; i < 4; i++) {
    fprintf(stdout, "\tinode %d - %s\n", i, (sb.s_inodes[i] == RD_INODE_FREE ? "RD_INODE_FREE" : "RD_INODE_INUSE"));
  }
}

void process_superblock(int fd) {
  struct rdfs_superblock sb;
  read_superblock(fd, &sb);
  print_superblock(sb);
}

void unimplemented(char c) {
  fprintf(stdout, "command '%c' not implemented yet\n", c);
}

int loop(int fd) {
  char c[10];
  while(1) {
    fprintf(stdout, "rdfs>");
    scanf("%s", c);
    switch(c[0]) {
    case 'q':
      fprintf(stdout, "exiting...\n");
      return 1;
      break;
    case 's':
      process_superblock(fd);
      break;
    case 'i':
      unimplemented(c[0]);
      break;
    default:
      fprintf(stdout, "default");
      unimplemented(c[0]);
      break;
    }
  }
  return 0;
}
