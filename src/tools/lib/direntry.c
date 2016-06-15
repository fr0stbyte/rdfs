#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "direntry.h"

void write_directory(int fd, struct rdfs_dirent *dirent, int cnt, int pos) {
  char block[RD_BSIZE];
  int offset = (RD_FIRST_DATA_BLOCK + pos) * RD_BSIZE;
  memset(block, 0, RD_BSIZE);

  if(pwrite(fd, block, RD_BSIZE, offset) < 0) {
    fprintf(stderr, "failed to write zeros at offset %d\n", offset);
    perror("[write_directory::block]");
    exit(1);
  }

  if(pwrite(fd, dirent, cnt * sizeof(struct rdfs_dirent), offset) < 0) {
    fprintf(stderr, "failed to write directory structure at %d\n", offset);
    perror("[write_directory::dirent]");
    exit(1);
  }
}

void read_directory(int fd, int blk_pos) {
  char block[RD_BSIZE];
  int offset = (RD_FIRST_DATA_BLOCK + blk_pos) * RD_BSIZE;
  memset(block, 0, RD_BSIZE);
  if(pread(fd, block, RD_BSIZE, offset) < 0) {
      fprintf(stderr, "failed to read block %d\n", blk_pos);
      perror("[read_directory]");
      exit(1);
    }
  struct rdfs_dirent *dir  = (struct rdfs_dirent*)block;
  while(dir->d_inode != 0) {
    print_directory(dir);
    dir++;
  }
}

void print_directory(struct rdfs_dirent *dir) {
  fprintf(stdout, "directory:\n");
  fprintf(stdout, "\tinode: %d\n", dir->d_inode);
  fprintf(stdout, "\tname: %s\n", dir->d_name);
}
