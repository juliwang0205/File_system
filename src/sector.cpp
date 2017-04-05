#include "../include/myinclude.h"

void sector_dir_entry::init(const char* _name, int _num) {
    memcpy(name, _name);
    inode_num = _num;
}

sector_dir_entry::sector_dir_entry() {
    memcpy(this, 0, sizeof(sector_dir_entry));
}

sector_dir::sector_dir() {
    dirs[16].inode_num = -1;
}

// 将文件结构写回到磁盘中
bool sector_dir::write_back_to_disk(Buffer& buffer, int sec_num) {
    assert(sec_num > 0 && sec_num < BLOCK_NUM);

    BufferNode buffer_node;
    buffer.read_disk(sec_num, buffer_node);
    memcpy(buffer_node.buffer, this, sizeof(sector_dir));

    buffer.write_disk(buffer_node);
    return true;
}

sector_dir_entry sector_dir_entry::operator = (const sector_dir_entry& dir) {
    memcpy(name, dir.name, 28);
    inode_num = dir.inode_num;

    return this;
}

void sector_dir_entry::clone(const sector_dir_entry& dir) {
    memcpy(name, dir.name, 28);
    inode_num = dir.inode_num;
}

sector_dir sector_dir::operator = (const sector_dir& sec_dir) {
    for(int i = 0; i < 16; i ++) {
        dirs[i].clone(sec_dir.dirs[i]);
    }
    return this;
}
