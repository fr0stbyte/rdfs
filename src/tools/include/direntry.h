#ifndef _RDFS_LIB_DIRENTRY_H_
#define _RDFS_LIB_DIRENTRY_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "rdfs.h"

void write_directory(int fd, struct rdfs_dirent *dirent, int cnt, int pos);
void read_directory(int fd, int block);
void print_directory(struct rdfs_dirent *dir);

#endif
