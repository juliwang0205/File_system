#define SEC_SIZE 512    // 扇区大小
#define INODE_NUM 131072  // inode数量
#define BLOCK_NUM 131072  // 数据扇区数量
#define DISK  "../disk.img"  // 模拟磁盘
#define SUPER_BEGIN 0     // 超级块起始地址
#define INODE_BEGIN sizeof(superblock)
#define BLOCK_BEGIN (sizeof(superblock) + sizeof(Inode) * INODE_NUM)
#define MAX_SEC   ((BLOCK_BEGIN + BLOCK_NUM * SEC_SIZE) / SEC_SIZE ) 

#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
