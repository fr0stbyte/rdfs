#include "lib.h"
#include "../module/include/rdfs.h"

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
  write_inode(devfd, 3, RD_BSIZE);

  // setup the inode for lost+found
  write_inode(devfd, 2, 2 * RD_BSIZE);
  // write each inode
  // skip into file section
  // write directories for ., .. and lost+found
  // write dirs

  return 0;
}
