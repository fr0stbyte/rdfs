#ifndef _RDFS_MKFS_H_
#define _RDFS_MKFS_H_

int open_device(char* device);
void verify_device_space(int fd);
void write_superblock(int fd);
void set_inode(int fd, int nlinks, off_t offset);

#endif
