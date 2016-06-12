#ifndef _RDFS_LIB_INODE_H_
#define _RDFS_LIB_INODE_H_

#include<unistd.h>
#include<time.h>

#include "rdfs.h"

void read_inode(int fd, struct rdfs_inode *inode, int pos);
void write_inode(int fd, int nlinks, int pos);
void print_inode(struct rdfs_inode inode);

#endif
