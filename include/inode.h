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
