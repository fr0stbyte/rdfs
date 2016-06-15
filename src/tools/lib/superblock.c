#include<stdio.h>
#include<stdlib.h>

#include "superblock.h"
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
    fprintf(stderr, "this is not a rdfs filesystem. Got %x, expected %x\n", sb.s_magic, RD_MAGIC);
    exit(1);
  }
  fprintf(stdout, "superblock: \n");
  fprintf(stdout, "\tmagic: 0x%x\n", sb.s_magic);
  fprintf(stdout, "\tmod: %s\n", (sb.s_mode == RD_FSCLEAN ? "RD_FSCLEAN": "RD_FSDIRTY"));
  fprintf(stdout, "\tfree blocks: %du\n", sb.s_nbfree);
  fprintf(stdout, "\tfree inodes: %du\n", sb.s_nifree);

  for(i = 0; i < 4; i++) {
    fprintf(stdout, "\tinode %d - %s\n", i, (sb.s_inodes[i] == RD_INODE_FREE ? "RD_INODE_FREE" : "RD_INODE_INUSE"));
  }
}

void write_superblock(int fd) {
  struct rdfs_superblock sb;
  int i = 0;

  sb.s_mode = RD_FSCLEAN;
  sb.s_magic = RD_MAGIC;
  sb.s_nifree = RD_MAXFILES - 4; // reserve space for iblock 0,1, / and lost+found
  sb.s_nbfree = RD_MAXBLOCKS - 2; // first 2 blocks are allocated for / and lost+found

  // reserve the first 4 inodes
  for(i = 0; i < 4; i++) {
    sb.s_inodes[i] = RD_INODE_INUSE;
  }

  //set the rest of inodes free
  for(i = 4; i < RD_MAXFILES; i++) {
    sb.s_inodes[i] = RD_INODE_FREE;
  }

  //set the first 2 blocks as used
  sb.s_blocks[0] = RD_BLOCK_INUSE;
  sb.s_blocks[1] = RD_BLOCK_INUSE;

  //set the reminder blocks as free
  for(i = 2; i < RD_MAXBLOCKS; i++) {
    sb.s_blocks[i] = RD_BLOCK_FREE;
  }

  // write at offset 0
  if(pwrite(fd, (void*)&sb, sizeof(sb), 0) < 0) {
    fprintf(stderr, "failed to write superblock\n");
    perror("[write_superblock]");
    exit(1);
  }
}
