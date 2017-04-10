/*******************************************
  总控程序
*******************************************/
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "myinclude.h"
using namespace std;


class my_fs{
public:
  // 显示模拟的shell界面
  void myshell();

  // 开始
  void run();

  // construct
  my_fs();
private:
    // 超级块
    superblock sp;

    // 当前所在的目录
    Inode cur_dir_node;
    sector_dir cur_dir;

    // 缓存
    Buffer my_cache;

    // 显示帮助信息
    void help(string command, string args = "");

    // 显示菜单
        void show_manu();

    // 读取下一行
    bool new_line(string &command);

    // 处理输入的命令,如果输入 exit 则返回false
    bool deal_input(const vector<string> command);

    // help_ls
    void deal_help(string args = "");

    // 格式化文件系统
    bool format_file_system();

    // ls: 列出文件夹下所有文件夹及目录(cur_dir中所有的信息)
    bool list_dir();

    // cd: 进入某个文件夹
    bool change_dir(const char* name);

    // mkdir: 创建文件夹
    bool make_dir(const char* name);

    // touch: 创建文件
    bool touch(const char* name);

    // rm: 删除文件或文件夹
    bool remove_dir(const char* name);

    // 根据删除inode
    bool del_inode(Inode& node, sector_dir& del_dir);

    // 将现成文件存入当前目录中
    bool move_in();

    // 如果当前目录有指定文件，就将这个文件从文件系统中读出
    bool move_out(string name);

    // 载入磁盘
    bool init_file_system();
};
