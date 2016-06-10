#ifndef _RDFS_RDSF_H_
#define _RDFS_RDSF_H_

#define RD_BSIZE     512
#define RD_DIRECT_BLOCKS 16  // max file size 8KB
#define RD_MAXFILES 32 // inodes : 0,1 reserved, 2 root, 3 lost+found - regular files
#define RD_MAXBLOCKS 468 // to keep superblock size under 512 bytes
// #define RD_PARTITION_MAXSIZE ((RD_MAXFILES * RD_DIRECT_BLOCKS + 1 ) * RD_BSIZE) // total blocks + superblock

#define __KERNEL__
#include<asm/types.h>

#define RD_MAGIC 0x00c0ffee

enum device_modes { RD_FSCLEAN, RD_FSDIRTY };
enum inode_modes { RD_INODE_FREE, RD_INODE_INUSE };
enum block_modes { RD_BLOCK_FREE, RD_BLOCK_INUSE };

struct rdfs_superblock {
  __u8 s_mod;
  __u32 s_magic;
  __u16 s_nbfree;
  __u16 s_nifree;
  __u8 s_inodes[RD_MAXFILES];
  __u8 s_blocks[RD_MAXBLOCKS];
};

struct rdfs_inode {
  __u32 i_mode;
  __u32 i_atime;
  __u32 i_ctime;
  __u32 i_mtime;
  __u32 i_uid;
  __u32 i_gid;
  __u32 i_size;
  __u32 i_blocks;
  __u32 i_direct_blocks[RD_DIRECT_BLOCKS];
};

#endif
