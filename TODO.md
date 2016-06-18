superblock:
  - extract mount options from the passed in data pointer in rdfs_fill_super

cmds:
  - be able to `cd` into the mount point -- create the proper inode/dentry
  - be able to `ls` files in that directory . , .., lost+found
  - be able to `cd` into `lost+found`
  - make `df` show the right disk size
