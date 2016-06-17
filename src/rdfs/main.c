#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/time.h>
#include<linux/types.h>
#include<linux/buffer_head.h>
#include<linux/dcache.h>

#include "rdfs.h"
#include "internal.h"

static struct inode* rdfs_get_inode(struct super_block *sb, const struct inode *dir,
                                    umode_t mode) {
  struct inode* inode;
  inode = new_inode(sb);
  if(inode) {
    inode->i_ino = get_next_ino();
    inode_init_owner(inode, dir, mode);
    inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
    switch(mode & S_IFMT) {
    case S_IFDIR:
      inode->i_op = &rdfs_inode_operations;
      inode->i_fop = &simple_dir_operations;
      break;
    }
  }
  return inode;
}

// initial the superblock for the system
static int rdfs_fill_super(struct super_block *sb, void *data, int silent){
  struct buffer_head *bh;
  struct inode *root_inode;
  struct rdfs_superblock *rdsb;
  int ret = -EPERM; //defaults to unable to read filesystem

  // read block from disk
  bh = sb_bread(sb, RD_SUPERBLOCK_POS);

  rdsb = (struct rdfs_superblock*)bh->b_data;

  printk(KERN_INFO
         "read superblock with magic : %x\n", rdsb->s_magic);

  if(unlikely(rdsb->s_magic != RD_MAGIC)) {
    printk(KERN_ERR
           "unsuitable filesystem found. magic number not understood\n");
    goto release;
  }

  sb->s_magic = rdsb->s_magic;
  sb->s_maxbytes = RD_BSIZE;
  sb->s_op = &rdfs_super_ops; // need to define structure with superblock operations
  sb->s_fs_info = (void*)rdsb;

  // might need to set a in-memory representation of our superblock so that we can access other information

  root_inode = rdfs_get_inode(sb, NULL, S_IFDIR);

  // set root node
  sb->s_root = d_make_root(root_inode);
  if(!sb->s_root) {
    ret = -ENOMEM;
    printk(KERN_ERR
           "failed to make root inode\n");
    goto release;
  }
  ret = 0;
  // if not allocated use iput
  //pretty much done unless need to parse some options
 release:
  brelse(bh);
  printk(KERN_INFO
         "returning %d from fill_super\n", ret);
  return ret;
}

// called when mount -t rdfs is handed over to rdfs module
static struct dentry* rdfs_mount(struct file_system_type *fs_type,
                          int flags, const char *dev_name,
                          void *data) {
  struct dentry *ret;

  ret = mount_bdev(fs_type, flags, dev_name, data, rdfs_fill_super);
  if(unlikely(IS_ERR(ret))) {
    printk(KERN_INFO
           "failed call to mount_bdev\n");
  } else {
    printk(KERN_INFO
           "successfully called rdfs_mount\n");
  }
  return ret;
}

// called when umount is called -- should destroy any data structures created for this particular filesystem ( caches, journals, etc)
// anything that was allocated via a kzalloc or similar
// private data in the superblock needs to be released
// if nothing special, can be replaced with the canned superblock killer : kill_block_super(struct super_block *sb)
static void rdfs_kill_superblock(struct super_block *sb) {
  printk(KERN_INFO
         "called rdfs_kill_superblock\n");
  kill_block_super(sb);
}

// structure describing the RDFS filesystem -- used to determine which module handles this type of fs
struct file_system_type rdfs_fs_type = {
  .owner = THIS_MODULE,
  .name = "rdfs",
  .mount = rdfs_mount,
  .kill_sb = rdfs_kill_superblock,
  .fs_flags = FS_REQUIRES_DEV
};


// called when the module is loaded
static int __init rdfs_init(void) {
  int ret = register_filesystem(&rdfs_fs_type);
  if(likely(ret == 0)) {
    printk(KERN_INFO
           "successfully registered rdfs\n");
  } else {
    printk(KERN_INFO
           "failed to register rdfs [err: %d]\n", ret);
  }
  return ret;
}

// called when the module is unloaded
static void __exit rdfs_exit(void) {
  int ret;
  printk(KERN_INFO "successfully unregistered rdfs\n");
  ret = unregister_filesystem(&rdfs_fs_type);
  if(likely(ret == 0)) {
    printk(KERN_INFO
           "successfully unregistered rdfs\n");
  } else {
    printk(KERN_INFO
           "failed to unregister module rdfs [err: %d]\n", ret);
  }
}

// macros to handle hook registration
module_init(rdfs_init);
module_exit(rdfs_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Radu Brumariu <brum76@gmail.com>");
