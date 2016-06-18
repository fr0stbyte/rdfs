#include<linux/fs.h>
#include<linux/dcache.h>

#include "rdfs.h"

//allocate an inode, result in dir/dentry
static int rdfs_inode_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev) {
  printk(KERN_INFO
         "rdfs_inode_mknod called");
  int error = -ENOSPC;
  struct inode *inode = rdfs_get_inode(dir->i_sb, dir, mode, dev);
  if(inode) {
    d_instantiate(dentry, inode);
    dget(dentry);
    error = 0;
    dir->i_mtime = dir->i_ctime = CURRENT_TIME;
  }
  return error;
}

static int rdfs_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev) {
  printk(KERN_INFO
         "rdfs_inode_create called");
  return rdfs_inode_mknod(dir, dentry, mode, dev)
}

const struct inode_operations rdfs_inode_operations = {
  .create = rdfs_inode_create,
  .mknod = rdfs_inode_mknod
};
