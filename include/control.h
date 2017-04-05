/*******************************************
  总控程序
*******************************************/
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
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

  // 缓存

};
