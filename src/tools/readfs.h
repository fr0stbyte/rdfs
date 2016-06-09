#ifndef _RDFS_READFS_H_
#define _RDFS_READFS_H_

#include "../module/include/rdfs.h"

int open_device(char *dev);
struct rdfs_superblock* read_superblock(int fd);
void print_superblock(struct rdfs_superblock *sb);

#endif
