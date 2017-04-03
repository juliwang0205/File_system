/*******************************************
  superblock类,占用64个扇区，262144个字节

*******************************************/
#include "myinclude.h"

class superblock{
private:
  bool inode_bitmap[INODE_NUM];
  bool block_botmap[BLOCK_NUM];
};
