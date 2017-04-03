/*******************************************
  扇区类,分为数据和目录两类

*******************************************/
#include "myinclude.h"

// 32 Bytes
struct sector_dir_entry{
  char name[28];
  int inode_num;
};

// 512 Bytes
class sector_dir{
private:
  sector_dir_entry dirs[16];
};

// 512 Bytes
class sector_file{
private:
  char data[508];
  int next;
};
