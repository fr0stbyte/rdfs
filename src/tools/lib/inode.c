#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

#include "inode.h"

void read_inode(int fd, struct rdfs_inode* inode, int pos) {
  off_t offset = RD_BSIZE * (pos + 1);
  fprintf(stdout, "reading inode from offset: %ld\n", offset);
  if(pread(fd, (struct rdfs_inode*)inode, sizeof(struct rdfs_inode), offset) < 0) {
    fprintf(stderr, "failed to read inode %d\n", pos);
    perror("[read_inode]");
    exit(1);
  }
}

void print_inode(struct rdfs_inode inode) {
  fprintf(stdout, "inode: \n");
  fprintf(stdout, "\tmode: %d\n", inode.i_mode);
  fprintf(stdout, "\tnlink: %d\n", inode.i_nlink);
  fprintf(stdout, "\tctime: %d\n", inode.i_ctime);
}

void write_inode(int fd, int nlinks, int pos) {
  struct rdfs_inode inode;
  time_t tm;

  off_t offset = pos * RD_BSIZE;

  time(&tm);
  memset(&inode, 0, sizeof(struct rdfs_inode));
  inode.i_mode = S_IFDIR | 0755;
  inode.i_nlink = nlinks;
  inode.i_atime = tm;
  inode.i_mtime = tm;
  inode.i_ctime = tm;
  inode.i_uid = 0; // uid/gid are superuser for / and lost+found
  inode.i_gid = 0;
  inode.i_size = RD_BSIZE;
  inode.i_blocks = 1;

  lseek(fd, offset, SEEK_SET);
  fprintf(stdout, "writing inode at offset %ld\nmode %d\natime: %d\n", offset, inode.i_mode, inode.i_atime);
  if(write(fd, &inode, sizeof(struct rdfs_inode)) < 0) {
    fprintf(stderr, "failed to write inode for root\n");
    perror("[set_root_inode]");
    exit(1);
  }
}
