#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include <iostream>
#define SEC_SIZE 512    // 扇区大小
#define INODE_NUM 131072  // inode数量
#define BLOCK_NUM 131072  // 数据扇区数量
#define DISK  "disk.img"  // 模拟磁盘
#define SUPER_BEGIN 0     // 超级块起始地址
#define INODE_BEGIN sizeof(superblock)
#define BLOCK_BEGIN (sizeof(superblock) + sizeof(Inode) * INODE_NUM)
#define MAX_SEC   ((BLOCK_BEGIN + BLOCK_NUM * SEC_SIZE) / SEC_SIZE )

using namespace std;
/*******************************************
  superblock类,占用64个扇区，262144个字节
*******************************************/

class superblock{
private:
  bool inode_bitmap[INODE_NUM];
  bool block_botmap[BLOCK_NUM];
};

/*******************************************
  缓存类, 模拟磁盘的IO全部从这里经过。
  会动态地检查节点的状态，将长时间没用的缓存写回磁盘
*******************************************/
struct BufferNode{
  char buffer[SEC_SIZE + 1];
  int pri;
  int sec_num;
  BufferNode operator = (const BufferNode& b) {
    memcpy(buffer, b.buffer, SEC_SIZE + 1);
    pri = b.pri;
    sec_num = b.sec_num;
  }
  BufferNode(){
    memset(buffer, 0, SEC_SIZE);
    pri = 0;
    sec_num = 0;
  }
  void init(int _sec_num) {
    pri = 5;
    sec_num = _sec_num;
  }
  void update(const BufferNode& b) {
    memcpy(buffer, b.buffer, SEC_SIZE + 1);
    pri = b.pri + 1;
    sec_num = b.sec_num;
  }
};

class Buffer{
  public:
      // 构造函数,打开文件
      Buffer();

      // 析构函数，关闭函数
      ~Buffer();

      // 将 buffer 里面的内容写入扇区中.单位为512KB。放入缓存队尾部。
      // 如果扇区已经存在于缓存中，则刷新扇区
      bool write_disk(const BufferNode& node);

      // 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
      // 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
      bool read_disk(int sec_num, BufferNode& node);

  // private:

    // 真正操作文件
    bool real_disk_write(const BufferNode& node);

    // 真正操作文件
    bool real_disk_read(int sec_num, BufferNode& node);

    // 检查缓存中有没有给定扇区号的缓存
    int has_sec(int sec_number);

    // 返回优先级最低的缓存号码,没满返回0
    int is_full();


    // 磁盘缓存，共15个节点，满了之后会将优先级最低的节点写回磁盘
    vector<BufferNode> cache;
    // 静态的文件指针
    fstream disk;
};


/*******************************************
  inode类，包括这样几个信息

  1. inode号码，这与目录中的inode号码对应
  2. 文件类型，是目录还是文件
  3. 文件大小，文件的具体大小
  4. 文件起始扇区号
  5. 文件占用的扇区数量

*******************************************/
// compensate to 32 Bytes
class Inode{
private:
  int inode_num;
  bool is_file;
  unsigned int file_size; // 最大4GB
  int sec_beg; // 只指定第一个，后面的通过指针连接
  int sec_num; // 占用的扇区数量
  char compensate[12];
};

/*******************************************
  扇区类,分为数据和目录两类
*******************************************/
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
