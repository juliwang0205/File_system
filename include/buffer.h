/*******************************************
  缓存类, 模拟磁盘的IO全部从这里经过。
  会动态地检查节点的状态，将长时间没用的缓存写回磁盘
*******************************************/
#include "myinclude.h"

struct BufferNode{
  char buffer[SEC_SIZE];
  int pri;
  int sec_num
};

class Buffer{
  public:
      // 构造函数,打开文件
      Buffer();

      // 析构函数，关闭函数
      ~Buffer();

      // 将 buffer 里面的内容写入扇区中.单位为512KB。放入缓存队尾部。
      // 如果扇区已经存在于缓存中，则刷新扇区
      bool write_disk(int sec_num, const BufferNode& node);

      // 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
      // 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
      bool read_disk(int sec_num, BufferNode& node);

  private:
    // 磁盘缓存，共15个节点，满了之后会将优先级最低的节点写回磁盘
    BufferNode cache[15];
    // 静态的文件指针
    fstream disk;
};
