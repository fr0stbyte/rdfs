#include<linux/fs.h>

#include "internal.h"

static int rdfs_statfs(struct dentry *dir, struct kstatfs *buf) {
  printk(KERN_INFO
         "rdfs_statfs called\n");
  return 0;
}

const struct super_operations rdfs_super_ops = {
  .statfs = rdfs_statfs,
};
