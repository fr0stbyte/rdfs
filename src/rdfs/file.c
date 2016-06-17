#include "rdfs.h"

static int rdfs_file_open(struct inode *inode, struct file *file) {
  printk(KERN_INFO
         "called rdfs_file_open");
  return 0;
}

const struct file_operations rdfs_file_operations = {
  .open = rdfs_file_open
};
