#include<linux/fs.h>
#include<linux/statfs.h>
#include<linux/slab.h>

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

static struct kmem_cache *rdfs_inode_cachep;

static struct inode * rdfs_super_alloc_inode(struct super_block *sb) {
  struct rdfs_inode_info *rdi;
  printk(KERN_INFO
         "rdfs_super_alloc_inode called");
  rdi = kmem_cache_alloc(rdfs_inode_cachep, GFP_KERNEL);
  if(!rdi)
    return NULL;

  rdi->vfs_inode.i_version = 1;
  return &rdi->vfs_inode;
}

static void rdfs_super_destroy_inode(struct inode *i) {
  printk(KERN_INFO
         "rdfs_super_destroy_inode called");
}

static void rdfs_super_dirty_inode(struct inode *i, int flags) {
  printk(KERN_INFO
         "rdfs_super_dirty_inode called");
}

static int rdfs_super_write_inode(struct inode *i, struct writeback_control *wbc) {
  printk(KERN_INFO
         "rdfs_super_write_inode called");
  return 0;
}

static int rdfs_super_drop_inode(struct inode* i) {
  printk(KERN_INFO
         "rdfs_super_drop_inode called");
  return 0;
}

static void rdfs_super_evict_inode(struct inode* i) {
  printk(KERN_INFO
         "rdfs_super_evict_inode called");
}

static void rdfs_super_put_super(struct super_block *sb) {
  printk(KERN_INFO
         "rdfs_put_super called");
}

static int rdfs_super_sync_fs(struct super_block *sb, int wait) {
  printk(KERN_INFO
         "rdfs_super_sync_fs called");
  return 0;
}

static int rdfs_super_freeze_fs(struct super_block *sb) {
  printk(KERN_INFO
         "rdfs_super_freeze_fs called");
  return 0;
}

static int rdfs_super_unfreeze_fs(struct super_block *sb) {
  printk(KERN_INFO
         "rdfs_super_unfreeze_fs called");
  return 0;
}

static int rdfs_super_remount_fs(struct super_block *sb, int * a, char *c) {
  printk(KERN_INFO
         "rdfs_super_remount_fs called");
  return 0;
}

static void rdfs_super_umount_begin(struct super_block *sb) {
  printk(KERN_INFO
         "rdfs_super_umount_begin called");
}

const struct super_operations rdfs_super_ops = {
  .statfs = rdfs_statfs,
  .alloc_inode = rdfs_super_alloc_inode,
  .destroy_inode = rdfs_super_destroy_inode,
  .dirty_inode = rdfs_super_dirty_inode,
  .write_inode = rdfs_super_write_inode,
  .drop_inode = rdfs_super_drop_inode,
  .evict_inode = rdfs_super_evict_inode,
  .put_super = rdfs_super_put_super,
  .sync_fs = rdfs_super_sync_fs,
  .freeze_fs = rdfs_super_freeze_fs,
  .unfreeze_fs = rdfs_super_unfreeze_fs,
  .remount_fs = rdfs_super_remount_fs,
  .umount_begin = rdfs_super_umount_begin,
};
