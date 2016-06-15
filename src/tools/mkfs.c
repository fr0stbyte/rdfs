#include<fcntl.h>

#include "mkfs.h"

int main(int argc, char **argv) {
  int devfd;

  // get the device from argv
  if(argc != 2) {
    fprintf(stderr, "need to pass in a device\n");
    exit(1);
  }
  // open device
  devfd = open_device(argv[1], O_WRONLY);

  // find if there is enough space -- seek blocks * block size
  verify_device_space(devfd);

  // write the superblock
  write_superblock(devfd);

  // setup the inodes for /
  write_inode(devfd, 3, 1);

  // setup the inode for lost+found
  write_inode(devfd, 2, 2);

  // skip into file section
  // write directories for / ., ..
  struct rdfs_dirent *p = calloc(3, sizeof(struct rdfs_dirent));
  struct rdfs_dirent *dirent = p;
  p->d_inode = 1;
  strcpy(p->d_name, ".");
  p++;
  p->d_inode = 1;
  strcpy(p->d_name, "..");
  p++;
  p->d_inode = 2;
  strcpy(p->d_name, "lost+found");
  write_directory(devfd, dirent, 3, 0);
  free(dirent);

  // write directories for lost+found ., ..
  p = calloc(2, sizeof(struct rdfs_dirent));
  dirent = p;
  p->d_inode = 2;
  strcpy(p->d_name, ".");
  p++;
  p->d_inode = 1;
  strcpy(p->d_name, "..");
  write_directory(devfd, dirent, 2, 1);

  free(dirent);

  return 0;
}
