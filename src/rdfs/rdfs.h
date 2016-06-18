#ifndef _RDFS_RDSF_H_
#define _RDFS_RDSF_H_

#define RD_BSIZE     512
#define RD_DIRECT_BLOCKS 16  // max file size 8KB
#define RD_MAXFILES 32 // inodes : 0,1 reserved, 2 root, 3 lost+found - regular files
#define RD_MAXBLOCKS 468 // to keep superblock size under 512 bytes
// #define RD_PARTITION_MAXSIZE ((RD_MAXFILES * RD_DIRECT_BLOCKS + 1 ) * RD_BSIZE) // total blocks + superblock
#define RD_NAMELEN 32 //maximum name length
#define RD_FIRST_DATA_BLOCK 33
#define RD_SUPERBLOCK_POS 0
#define RD_ROOT_INODE_NUMBER 1

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include<asm/types.h>

#define RD_MAGIC 0x00c0ffee

enum device_modes { RD_FSCLEAN, RD_FSDIRTY };
enum inode_modes { RD_INODE_FREE, RD_INODE_INUSE };
enum block_modes { RD_BLOCK_FREE, RD_BLOCK_INUSE };

// ondisk superblock data structure
// 512 bytes
struct rdfs_superblock {
  __u8 s_mode;
  __u32 s_magic;
  __u16 s_nbfree;
  __u16 s_nifree;
  __u8 s_inodes[RD_MAXFILES];
  __u8 s_blocks[RD_MAXBLOCKS];
};

// on disk inode data structure
// 102 bytes
struct rdfs_inode {
  __u16 i_mode;
  __u8 i_nlink;
  __u32 i_atime;
  __u32 i_ctime;
  __u32 i_mtime;
  __u8 i_uid;
  __u8 i_gid;
  __u32 i_size;
  __u32 i_blocks;
  __u32 i_data[RD_DIRECT_BLOCKS];
};

// on disk directory structure

struct rdfs_dirent {
  __u32 d_inode;
  char d_name[RD_NAMELEN];
};


// in memory inode structure

struct rdfs_inode_info {
  // position on disk of inode
  __u32 inode_location;
  struct inode vfs_inode;
};
extern const struct inode_operations rdfs_inode_operations;
extern const struct file_operations rdfs_file_operations;

#endif
