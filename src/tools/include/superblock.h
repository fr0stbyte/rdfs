#ifndef _RDFS_LIB_SUPERBLOCK_H_
#define _RDFS_LIB_SUPERBLOCK_H_

#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#include "rdfs.h"

void read_superblock(int fd, struct rdfs_superblock *sb);
void write_superblock(int fd);
void print_superblock(struct rdfs_superblock sb);

#endif
