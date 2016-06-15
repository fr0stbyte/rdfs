#include<linux/fs.h>
#include<linux/statfs.h>

#include "rdfs.h"
#include "internal.h"

static int rdfs_statfs(struct dentry *dir, struct kstatfs *buf) {
  struct super_block *sb;
  struct rdfs_superblock *rsb;
  printk(KERN_INFO
         "rdfs_statfs called\n");

  sb = dir->d_sb; // get the superblock
  rsb = (struct rdfs_superblock*)sb->s_fs_info;

  // start filling in the buf structure
  buf->f_type = RD_MAGIC;
  buf->f_bsize = sb->s_blocksize;
  buf->f_blocks = RD_MAXBLOCKS;
  buf->f_bfree = rsb->s_nbfree;
  buf->f_bavail = RD_MAXBLOCKS - rsb->s_nbfree;
  buf->f_files = RD_MAXFILES;
  buf->f_ffree = rsb->s_nifree;
  buf->f_namelen = RD_NAMELEN;

  return 0;
}

const struct super_operations rdfs_super_ops = {
  .statfs = rdfs_statfs,
};
