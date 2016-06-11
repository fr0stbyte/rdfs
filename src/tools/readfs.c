#include "lib.h"
#include "../module/include/rdfs.h"

int loop(int fd);

int main(int argc, char ** argv) {
  int devfd;

  if(argc != 2) {
    fprintf(stderr, "need to specify a device\n");
    exit(1);
  }

  devfd = open_device(argv[1], O_RDONLY);

  loop(devfd);

  return 0;
}

void process_superblock(int fd) {
  struct rdfs_superblock sb;
  read_superblock(fd, &sb);
  print_superblock(sb);
}

void process_inode(int fd, int n) {
  struct rdfs_inode inode;
  memset(&inode, 0, sizeof(struct rdfs_inode));
  read_inode(fd, &inode, n);
  print_inode(inode);
}

void unimplemented(char c) {
  fprintf(stdout, "command '%c' not implemented yet\n", c);
}

int loop(int fd) {
  char cmd[10];
  int j = 0;
  while(1) {
    fprintf(stdout, "rdfs>");
    scanf("%s", cmd);
    switch(cmd[0]) {
    case 'q':
      fprintf(stdout, "exiting...\n");
      return 1;
      break;
    case 's':
      process_superblock(fd);
      break;
    case 'i':
      scanf("%d", &j);
      process_inode(fd, j);
      break;
    default:
      fprintf(stdout, "default");
      unimplemented(cmd[0]);
      break;
    }
  }
  return 0;
}
