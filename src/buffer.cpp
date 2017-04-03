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
  assert(sec_num > 0 && sec_num < MAX_SEC);

  int i;
  if((i = has_sec(sec_num))) {
    cache[i].pri++;
    cache[i] = node;
    return true;
  }

  if(( i = is_full() )) {
    
  }
  else{

  }
  cache.push_back(node);

}

// 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
// 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
bool Buffer::read_disk(int sec_num, BufferNode& node){
  assert(sec_num > 0 && sec_num < MAX_SEC);

  int i;
  if((i = has_sec(sec_num))) {
    cache[i].pri++;
    node = cache[i];
    return true;
  }

  // 缓存中没有这个扇区
  if( ( i = is_full() ) ) {
    // 写回
    real_disk_write(cache[i]);
    cache.erase(cache.begin() + i);
    real_disk_read(sec_num, node);
    cache.push_back(node);
    node.pri = 5;
    node.sec_num = sec_num;
  }
  else {
    node.pri = 5;
    node.sec_num = sec_num;
    real_disk_read(sec_num, node);
    cache.push_back(node);
  }
  return true;
}

// 真正操作文件
bool Buffer::real_disk_write(const BufferNode& node){
  assert(node.sec_num > 0 && node.sec_num < MAX_SEC);
  disk.seekg(node.sec_num * SEC_SIZE, disk.beg);
  disk.write(disk.buffer, SEC_SIZE);
  node.sec_num = sec_num;
  return true;
}

// 真正操作文件
bool Buffer::real_disk_read(int sec_num, const BufferNode& node){
  assert(sec_num > 0 && sec_num < MAX_SEC);

  disk.seekg(sec_num * SEC_SIZE, disk.beg);
  disk.read(node.buffer, sec_SIZE);
  node.sec_num = sec_num;
  return true;
}


// 检查缓存中有没有给定扇区号的缓存
int Buffer::has_sec(int sec_number) {
  for(int i = 0; i < cache.size(); i++) {
    if(cache[i].sec_num == sec_number)
      return i;
  }
  return 0;
}

// 返回优先级最低的缓存号码
int Buffer::is_full() {
  if(cache.size() == 15){
    int min = -1, min_i = 0;
    for(int i = 0; i < cache.size(); i++) {
      if(cache[i].pri > min){
        min = cache[i].pri;
        min_i = i;
      }
    }
    return min_i;
  }
  else {
    return 0;
  }
}
