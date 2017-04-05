#include "../include/control.h"

namespace strtool
{
string trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

int split(const string& str, vector<string>& ret_, string sep = ",")
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

string replace(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}
}


// 显示帮助信息
void my_fs::help(string command, string args) {
  if(args == "") {
    show_manu();
  } else if(args == "ls"){
      cout << "还没有完成" << endl;

  }else if(args == "cd") {
    cout << "还没有完成" << endl;

  } else {
    cout << "命令错误，请输入 help 查看相关的命令的使用方法" << endl;

  }
}

// 读取下一行
bool my_fs::new_line(string &command) {
  cout << ">>> ";
  string tem;
  bool flag = false;
  while(getline(cin, tem)) {
    if(tem.empty()){
      break;
    } else {
      command = tem;
    }
    flag = true;
  }
  return flag;
}

// 显示模拟的shell界面
void my_fs::myshell(){
  string input, tem;
  vector<string> commands;
  while(1) {
    if(new_line(input) == true) {
      stringstream ss(input);
      while(ss >> tem)
        commands.push_back(tem);
      if(deal_input(commands) == false)
        break;
  }
    commands.clear();
  }
    return;
}

// 处理输入的命令,如果输入 exit 则返回false
bool my_fs::deal_input(const vector<string> command) {
  if(command.size() == 0)
    return true;
  string comm = command[0];
  if(comm == "help") {
    if(command.size() == 1)
      deal_help();
    else if(command.size() > 1)
      deal_help(command[1]);
  }
  else if(comm == "ls"){
    cout << "还没有完成ls命令" << endl;
  }
  else if(comm == "exit") {
    cout << "退出系统" << endl;
    return false;
  }
  else {
    cout << "没有这一条命令，请输入help查看相关帮助信息" << endl;
  }
  return true;
}

// help 命令
void my_fs::deal_help(string args){
  if(args == "") {
    show_manu();
  }
  else {
    if(args == "ls") {
      cout << "ls <文件或目录名>" << endl;
    }
    else if(args == "touch") {
      cout << "touch <文件名>" << endl;
    }
    else if(args == "mkdir") {
      cout << "touch <目录名>" << endl;
    }
    else if(args == "cd") {
      cout << "rm <目录名>" << endl;
    }
    else if(args == "rm") {
      cout << "rm <文件或目录名>" << endl;
    }
    else if(args == "format") {
        cout << "即将格式化磁盘，输入1确定，0取消" << endl;
    }
    else {
      cout << "无法识别这条命令" << endl;
    }
  }
}

// 开始
void my_fs::run() {
  myshell();
  return;
}

// 显示菜单
void my_fs::show_manu() {
  cout << "help 帮助信息" << endl;
  cout << "语法： help <指令>" << endl;
  cout << "如：help (显示所有帮助信息)" << endl;
  cout << "如：help ls (显示ls这条命令的帮助信息)" << endl << endl;

  cout << "exit: 退出系统" << endl;
  cout << "语法： exit" << endl;

}

// construct
my_fs::my_fs(){
  cout << endl << "****************** 文件系统启动 ******************" << endl;

}

// 初始化文件系统
bool my_fs::format_file_system() {
    /*
    *  1. 格式化superblock
    */
    sp.init();

    /* 2. 申请根目录的一系列inode。包括根目录
    *     根目录下面的bin、etc、home、dev
    *     home目录下面的tangrui目录
    */
    Inode root_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode bin_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode etc_root(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode home_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode dev_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode tangrui_root(sp.get_new_inode(), false, 0, sp.get_new_sec());

    /*
    *   3. 将inode写回到磁盘中
    */
    root_node.write_inode_back_to_disk(my_cache);
    bin_node.write_inode_back_to_disk(my_cache);
    etc_root.write_inode_back_to_disk(my_cache);
    home_node.write_inode_back_to_disk(my_cache);
    dev_node.write_inode_back_to_disk(my_cache);
    tangrui_root.write_inode_back_to_disk(my_cache);

    /*
    *   4. 建立数据扇区中的目录结构
    */
    sector_dir root_sec_dir;
    root_sec_dir.dirs[0].init(".", 1);
    root_sec_dir.dirs[1].init("..", 1);
    root_sec_dir.dirs[2].init("bin", bin_node.get_inode_num());
    root_sec_dir.dirs[3].init("etc", etc_root.get_inode_num());
    root_sec_dir.dirs[4].init("home", home_node.get_inode_num());
    root_sec_dir.dirs[5].init("dev", dev_node.get_inode_num());

    sector_dir bin_sec_dir;
    bin_sec_dir.dirs[0].init(".", bin_node.get_inode_num());
    bin_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir etc_sec_dir;
    etc_sec_dir.dirs[0].init(".", etc_root.get_inode_num());
    etc_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir home_sec_dir;
    home_sec_dir.dirs[0].init(".", home_node.get_inode_num());
    home_sec_dir.dirs[1].init("..", root_node.get_inode_num());
    home_sec_dir.dirs[1].init("tangrui", tangrui_node.get_inode_num());

    sector_dir dev_sec_dir;
    dev_sec_dir.dirs[0].init(".",  dev_node.get_inode_num());
    dev_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir tangrui_sec_dir;
    tangrui_sec_dir.dirs[0].init(".",  tangrui_node.get_inode_num());
    tangrui_sec_dir.dirs[1].init("..", home_node.get_inode_num());


    /*
    *  5. 将文件夹对应的扇区写入到磁盘中
    */
    root_sec_dir.write_back_to_disk(my_cache, root_node.get_sec_num());
    bin_sec_dir.write_back_to_disk(my_cache, bin_node.get_sec_num());
    etc_sec_dir.write_back_to_disk(my_cache, etc_node.get_sec_num());
    home_sec_dir.write_back_to_disk(my_cache, home_node.get_sec_num());
    dev_sec_dir.write_back_to_disk(my_cache, dev_node.get_inode_num());
    tangrui_sec_dir.write_back_to_disk(my_cache, tangrui_node.get_inode_num());

    /*
    *  6. 修改系统当前目录位置为根目录
    */
    cur_dir = root_sec_dir;
    cur_dir_node = root_node;
    return true;
}
