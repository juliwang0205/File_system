#include "../include/myinclude.h"

superblock::superblock() {
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
    memset(block_bitmap, 0, sizeof(block_bitmap));
}


// 剩余的inode数量
int superblock::remain_inode() {
    int count = 0;
    for(int i = 0; i < INODE_NUM; i++)
        if(!inode_bitmap[i])
            count++;
    return count;
}

// 剩余的扇区数量
int superblock::remain_sec() {
    int count = 0;
    for(int i = 0; i < INODE_NUM; i++)
        if(!block_bitmap[i])
            count++;
    return count;
}

// 返回未使用的i节点, 如果满了就返回-1
int superblock::get_new_inode() {
    for(int i = 0; i < INODE_NUM; i++)
        if(!inode_bitmap[i]){
            inode_bitmap[i] = true;
            return i;
        }
    return -1;
}

// 返回未使用的扇区,如果扇区已经用完就返回-1
int superblock::get_new_sec() {
    for(int i = 0; i < BLOCK_NUM; i++)
        if(!block_bitmap[i]){
            block_bitmap[i] = true;
            return i + INODE_BEGIN / 512 + (INODE_NUM * sizeof(Inode)) / 512;
        }
    return -1;
}

// 回收i节点
bool superblock::recv_inode(int inode_num) {
    assert(inode_num >= 0 && inode_num < INODE_NUM);

    inode_bitmap[inode_num] = false;
    return true;
}

// 回收扇区
bool superblock::recv_sec(int sec_num) {
    assert(sec_num >= 0 && sec_num < BLOCK_NUM);

    block_bitmap[sec_num] = false;
    return true;
}

// 格式化
bool superblock::init() {
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
    memset(block_bitmap, 0, sizeof(block_bitmap));
    return true;
}
