#ifndef _RDFS_LIB_H_
#define _RDFS_LIB_H_

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#include "../module/include/rdfs.h"

int open_device(const char *dev, int mode);
void read_superblock(int fd, struct rdfs_superblock *sb);
void print_superblock(struct rdfs_superblock sb);
void read_inode(int fd, struct rdfs_inode *inode, int pos);
void print_inode(struct rdfs_inode inode);
void verify_device_space(int fd);
void write_superblock(int fd);
void write_inode(int fd, int nlinks, off_t offset);

#endif
