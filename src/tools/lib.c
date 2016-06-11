#include "lib.h"

int open_device(const char *dev, int mode) {
  int fd;
  if((fd = open(dev, mode)) < 0) {
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

void write_inode(int fd, int nlinks, off_t offset) {
  struct rdfs_inode inode;
  time_t tm;

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
