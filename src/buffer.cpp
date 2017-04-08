#include "../include/myinclude.h"

Buffer::Buffer(){
  disk.open(DISK, std::fstream::in | std::fstream::out | std::fstream::binary);
  if(disk.is_open())
    cout << "文件已经打开" << endl;
  else
    cout << "文件没有打开" << endl;
}

Buffer::~Buffer(){
  disk.close();
}

// 将 buffer 里面的内容写入扇区中.单位为512KB。放入缓存队尾部。
// 如果扇区已经存在于缓存中，则刷新扇区
bool Buffer::write_disk(const BufferNode& node){
  assert(node.sec_num >= 0 && node.sec_num < MAX_SEC);
  int i;
  i = has_sec(node.sec_num);
  if(i >= 0) {
    cout << "write disk: 缓存中有这个扇区" << endl;
    cache[i].update(node);
    return true;
  }

  i = is_full();
  if(i >= 0) {
    cout << "write disk: 缓存已满，替换第" << i << "号缓存" << endl;
    // 缓存已满，将优先级最低的缓存写回磁盘
    real_disk_write(cache[i]);
    cache.erase(cache.begin() + i);
  }
  cache.push_back(node);
  cout << "write disk: 数据已经写入缓存中" << endl;
  return true;
}




// 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
// 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
bool Buffer::read_disk(int sec_num, BufferNode& node){
  assert(sec_num >= 0 && sec_num < MAX_SEC);

  int i;
  i = has_sec(sec_num);

  if(i >= 0) {
    cout << "read disk: 缓存中有该扇区" << endl;
    node.update(cache[i]);
    return true;
  }

  // 缓存中没有这个扇区
  i = is_full();
  if(i >= 0) {
    cout << "read disk: 缓存已满，替换第" << i << "号缓存" << endl;
    // 写回
    real_disk_write(cache[i]);
    cache.erase(cache.begin() + i);
    real_disk_read(sec_num, node);
    node.init(sec_num);
    cache.push_back(node);
  }
  else {
    real_disk_read(sec_num, node);
    node.init(sec_num);
    cache.push_back(node);
    cout << "read disk: 缓存不满，磁盘扇区已存入缓存" << endl;
  }
  return true;
}

// 真正写文件
bool Buffer::real_disk_write(const BufferNode& node){
  assert(node.sec_num >= 0 && node.sec_num < MAX_SEC);
  cout << "read disk write 写回第" << node.sec_num << "号扇区" << endl;
  disk.seekg(node.sec_num * SEC_SIZE, disk.beg);
  disk.write(node.buffer, SEC_SIZE);
  return true;
}

// 真正读文件
bool Buffer::real_disk_read(int sec_num, BufferNode& node){
  assert(sec_num >= 0 && sec_num < MAX_SEC);
  cout << "real disk read 读取第" << sec_num << "号扇区" << endl;
  disk.seekg(sec_num * SEC_SIZE, disk.beg);
  disk.read(node.buffer, SEC_SIZE);
  node.buffer[SEC_SIZE] = '\0';
  node.sec_num = sec_num;
  return true;
}


// 检查缓存中有没有给定扇区号的缓存
int Buffer::has_sec(int sec_number) {
  for(int i = 0; i < cache.size(); i++) {
    if(cache[i].sec_num == sec_number)
      return i;
  }
  return -1;
}

// 返回优先级最低的缓存号码
int Buffer::is_full() {
  if(cache.size() == 15){
    int min = 9999, min_i = 0;
    for(int i = 0; i < cache.size(); i++) {
      if(cache[i].pri < min){
        min = cache[i].pri;
        min_i = i;
      }
    }
    return min_i;
  }
  else {
    return -1;
  }
}
