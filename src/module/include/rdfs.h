#ifndef _RDFS_RDSF_H_
#define _RDFS_RDSF_H_

#define RD_BSIZE     512
#define RD_DIRECT_BLOCKS 2048  // max file size
#define RD_MAXFILES 1020 // inodes : 0,1 reserved, 2 root, 3 lost+found, 65532 - regular files
#define RD_MAXBLOCKS RD_DIRECT_BLOCKS * RD_MAXFILES - RD_MAXFILES - 1 // DIRECT_BLOCKS * MAX_FILES - 2^10 - 1 blocks (2^10 = space reserved for inodes, 1 space reserved for superblock)
#define RD_PARTITION_MAXSIZE (RD_MAXFILES * (RD_DIRECT_BLOCKS + 1) * RD_BSIZE)

#define __KERNEL__
#include<asm/types.h>

#define RD_MAGIC 0x7df52016

enum device_modes { RD_FSCLEAN, RD_FSDIRTY };
enum inode_modes { RD_INODE_FREE, RD_INODE_INUSE };
enum block_modes { RD_BLOCK_FREE, RD_BLOCK_INUSE };

struct rdfs_superblock {
  __u32 s_mod;
  __u32 s_magic;
  __u32 s_nbfree;
  __u32 s_nifree;
  __u32 s_inodes[RD_MAXFILES];
  __u32 s_blocks[RD_MAXBLOCKS];
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
