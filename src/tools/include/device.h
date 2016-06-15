#ifndef _RDFS_LIB_DEVICE_H_
#define _RDFS_LIB_DEVICE_H_

#include "rdfs.h"

int open_device(const char *dev, int mode);
void verify_device_space(int fd);

#endif
