#include "./include/buffer.h"

Buffer::Buffer(){
  disk.open(DISK, std::fstream::in | std::fstream::out | std::fstream::binary);

}

Buffer::~Buffer(){
  disk.close();
}

// 将 buffer 里面的内容写入扇区中.单位为512KB。放入缓存队尾部。
// 如果扇区已经存在于缓存中，则刷新扇区
bool Buffer::write_disk(int sec_num, const BufferNode& node){


}

// 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
// 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
bool Buffer::read_disk(int sec_num, BufferNode& node);
