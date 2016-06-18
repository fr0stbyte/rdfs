#include<linux/fs.h>

extern const struct super_operations rdfs_super_ops;

struct inode *rdfs_get_inode(struct super_block *sb, const struct inode *dir, umode_t mode);
