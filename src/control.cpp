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
    list_dir();
  }
  else if(comm == "exit") {
    cout << "退出系统" << endl;
    return false;
  }
  else if(comm == "cd") {
      const char* dir = command[1].c_str();
      change_dir(dir);
  }
  else if(comm == "reformat") {
      format_file_system();
  }
  else if(comm == "mkdir"){
      const char* dir = command[1].c_str();
      make_dir(dir);
  }
  else if(comm == "touch"){
      const char* file = command[1].c_str();
      touch(file);
  }
  else if(comm == "rm") {
      const char* del_name = command[1].c_str();
      remove_dir(del_name);
  } 
  else if(comm == "mv_in") {
     move_in(); 
  }
  else if(comm == "mv_out") {
      move_out(command[1]);
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
    else if(args == "reformat") {
        cout << "reformat: 初始化磁盘" << endl;
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

  cout << "reformat:初始化磁盘" << endl;

  cout << "ls:列出当前文件夹下所有文件和目录" << endl;

  cout << "cd：进入某个目录" << endl;

  cout << "exit: 退出系统" << endl;

}

// ls: 列出文件夹下所有文件夹及目录(cur_dir中所有的信息)
bool my_fs::list_dir() {
    for(int i = 2; i < 15; i++) {
        cout << cur_dir.dirs[i].name << " ";
    }
    cout << endl;
    return true;
}
// rm: 删除文件或文件夹
bool my_fs::remove_dir(const char* name) {
    // 1. 在当前目录下寻找这个文件或目录
    int del_inode_num = -1;
    for(int i = 0; i < 15; i++) {
        if(strncmp(name, cur_dir.dirs[i].name, strlen(name)) == 0) {
            del_inode_num = cur_dir.dirs[i].inode_num;
            cout << "该文件或目录inode号码为：" << del_inode_num << endl;
            break;
        }
    }
    if(del_inode_num == -1) {
        cout << "不存在这个文件或目录" << endl;
        return false;
    }

    // 2. 读取这个inode节点
    Inode del_node;
    del_node.read_inode_from_disk(del_inode_num, my_cache);

    // cout << "inode对应数据扇区号码为：" << del_node.get_sec_beg() - BLOCK_BEGIN / 512 << endl;
    // cout << "inode号为：" << del_node.get_inode_num() << endl;

    del_inode(del_node, cur_dir);

    return true;
}



// cd: 进入某个文件夹
bool my_fs::change_dir(const char* name) {
    // 1. 得到子目录的inode号码
    int dir_inode_num = -1;
    for(int i = 0; i < 15; i++) {
        if(strncmp(name, cur_dir.dirs[i].name, strlen(name)) == 0) {
            dir_inode_num = cur_dir.dirs[i].inode_num;
            cout << "目录inode号码为：" << dir_inode_num << endl;
            break;
        }
    }
    if(dir_inode_num == -1) {
        cout << "该目录不存在" << endl;
        return false;
    }

    // 2. 根据这个inode号码找到相应的inode
    cur_dir_node.read_inode_from_disk(dir_inode_num, my_cache);

    // 3. 根据inode中的信息区磁盘中读取目录信息
    cur_dir.read_dir_from_disk(my_cache, cur_dir_node.get_sec_beg());

    return true;
}

// 根据删除inode
bool my_fs::del_inode(Inode& node, sector_dir& del_dir) {
    cout << "delete inode, inode num为" << node.get_inode_num() << endl;
    if(node.get_type()) {
        // file, 释放inode，inode对应的sec， 还要从dir中去除这个项目
        //  1.删除sec中的这条记录
        for(int i = 2; i < 15; i++) {
            if(del_dir.dirs[i].inode_num == node.get_inode_num()) {
                cout << "delate inode，删除sector中对文件的记录" << endl;
                memset(&del_dir.dirs[i], 0, sizeof(sector_dir_entry));
                del_dir.write_back_to_disk(my_cache, node.get_sec_beg());
                break;
            }
        }

        // 2. 释放inode和对应的扇区
        sp.recv_sec(node.get_sec_beg() - BLOCK_BEGIN / 512);
        sp.recv_inode(node.get_inode_num());
    }
    else {
        // dir
        // 1.先删除当前目录对这个目录的记录
        for(int i = 0; i < 15; i++) {
            if(node.get_inode_num() == del_dir.dirs[i].inode_num) {
                cout << "delate inode，删除sector中对文件的记录" << endl;
                memset(&del_dir.dirs[i], 0, sizeof(sector_dir_entry));
                del_dir.write_back_to_disk(my_cache, node.get_sec_beg());
                break;
            }
        }
        // 2. 释放这个目录的inode和扇区
        sp.recv_sec(node.get_sec_beg() - BLOCK_BEGIN / 512);
        sp.recv_inode(node.get_inode_num());
        // 3. 切换到要删除的目录下
        Inode new_node;
        new_node = node;
        sector_dir new_dir;
        new_dir = del_dir;

        new_dir.read_dir_from_disk(my_cache, new_node.get_sec_beg());
        // 4. delete every files and directories recursively
        for(int i = 2; i < 15; i++) {
            if(new_dir.dirs[i].inode_num != 0) {
                new_node.read_inode_from_disk(new_dir.dirs[i].inode_num, my_cache);
                del_inode(new_node, new_dir);
            }
        }
    }
}


// 将现成文件存入当前目录中
bool my_fs::move_in() {
    /*
    *  move p1.png into my file system
    */
    // 1. get file size, compute needed block number, allocate block 
    ifstream is(IMG, ifstream::binary);
    if(is) {
        is.seekg(0, is.end);
        int length = is.tellg();
        cout << "size of the file:" << length << " bytes" << endl;

        // 2. compute needed blocks
        int needed_block = length / 508;
        if(length % 508 != 0) 
            needed_block++;
        int left = length % 508;
        cout << endl << "last node contain " << ((left == 0) ? 508 : left) << "bytes of data" << endl;
        cout << "need " << needed_block << " blocks to store data" << endl;
        
        Inode new_file_inode(sp.get_new_inode(), true, length, sp.get_new_sec());
        new_file_inode.write_inode_back_to_disk(my_cache);
        cout << "img inode info: #inode: " << new_file_inode.get_inode_num() << endl;
        cout << "file length " << new_file_inode.get_file_size() << endl;
        cout << " #sector begin: " << new_file_inode.get_sec_beg() << endl;

        // 3. add new entry in current directory
        int flag = false;
        for(int i = 2; i < 15; i++) {
            if(cur_dir.dirs[i].inode_num == 0) {
                cur_dir.dirs[i].init(IMG, new_file_inode.get_inode_num());
                flag = true;
                break;
            }
        }
        if(flag) {
            cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());
        }

        // 4. store data into file system
        is.seekg(0, is.beg);
        char buffer[508];
        sector_file img_sectors[needed_block];
        int sec_numbers[needed_block];        
        sec_numbers[0] = new_file_inode.get_sec_beg();
        for(int i = 0; i < needed_block - 1; i++) {
            is.read(buffer, 508);
            sec_numbers[i+1] = sp.get_new_sec();
            memcpy(img_sectors[i].data, buffer, 508);
            img_sectors[i].next = sec_numbers[i+1];
            cout << "#next data sector:" << img_sectors[i].next << endl;
        }
        if(left == 0) {
            is.read(buffer, 508);
            memcpy(img_sectors[needed_block - 1].data, buffer, 508);
            img_sectors[needed_block - 1].next = 0;
        }
        else {
            is.read(buffer, left);
            memcpy(img_sectors[needed_block - 1].data, buffer, left);
            img_sectors[needed_block - 1].next = 0;
        }

        cout << "File pointer location" << is.tellg() << endl;
        cout << "file sectors info" << endl;
        cout << new_file_inode.get_sec_beg();
        for(int i = 0; i < needed_block; i++) {
            cout << " -> " << img_sectors[i].next;
        }
        cout << endl;
        for(int i = 0; i < needed_block; i++) {
            img_sectors[i].write_back_to_disk(my_cache, sec_numbers[i]);
        }        

        is.close(); 
    }
     
}

// 如果当前目录有指定文件，就将这个文件从文件系统中读出
bool my_fs::move_out(string name) {
    /*
    * move p1.png out of my file system
    */

    // 1. search for inode number
    int inode_num = -1;
    for(int i = 0; i < 15; i++){ 
        if(strncmp(IMG, cur_dir.dirs[i].name, strlen(IMG)) == 0) {
            inode_num = cur_dir.dirs[i].inode_num;
            cout << "inode of p1.png: " << inode_num << endl;
            break;
        }
    }
    if(inode_num == -1) {
        cout << "pl.png not exists" << endl;
        return false;
    }
    Inode file_node;
    file_node.read_inode_from_disk(inode_num, my_cache);

    cout << "file info: #inode " << file_node.get_inode_num() << endl;
    cout << "file length: " << file_node.get_file_size() << endl;
    cout << "sec number: " << file_node.get_sec_num() << endl;
    cout << "sec_begin: " << file_node.get_sec_beg() << endl << endl;

    // 2. get data of p1.png from my file system 
    sector_file data_sec;
    data_sec.read_dir_from_disk(my_cache, file_node.get_sec_beg());
    string file_name = name + ".png";
    fstream os(file_name.c_str(), fstream::in | fstream::out | fstream::app);

    char buffer[508];
    int next_sec = -1, left = file_node.get_file_size() % 508;
    if(os) {
        for(int i = 0; i < file_node.get_sec_num() ; i++) {
            if(i != file_node.get_sec_num() - 1 || left == 0) {
                next_sec = data_sec.next;
                memcpy(buffer, data_sec.data, 508);
                os.write(buffer, 508);
                data_sec.read_dir_from_disk(my_cache, next_sec);
            }
            else {
                memcpy(buffer, data_sec.data, left);
                os.write(buffer, left);
            }
            cout << "size of new file:" << os.tellg() << endl;
        }        
        os.close();
    }

    return true;
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
    cout << "******************************** 初始化磁盘开始 ********************************" << endl;
    sp.init();

    /*
    * 2. 申请根目录的一系列inode。包括根目录
    *     根目录下面的bin、etc、home、dev
    *     home目录下面的tangrui目录
    */
    Inode root_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode bin_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode etc_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode home_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode dev_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode tangrui_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    cout << "1. 申请inode" << endl;
    /*
    *   3. 将inode写回到磁盘中
    */
    root_node.write_inode_back_to_disk(my_cache);
    bin_node.write_inode_back_to_disk(my_cache);
    etc_node.write_inode_back_to_disk(my_cache);
    home_node.write_inode_back_to_disk(my_cache);
    dev_node.write_inode_back_to_disk(my_cache);
    tangrui_node.write_inode_back_to_disk(my_cache);
    cout << "2. inode写回磁盘" << endl;
    /*
    *   4. 建立数据扇区中的目录结构
    */
    sector_dir root_sec_dir;
    root_sec_dir.dirs[0].init(".", 1);
    root_sec_dir.dirs[1].init("..", 1);
    root_sec_dir.dirs[2].init("bin", bin_node.get_inode_num());
    root_sec_dir.dirs[3].init("etc", etc_node.get_inode_num());
    root_sec_dir.dirs[4].init("home", home_node.get_inode_num());
    root_sec_dir.dirs[5].init("dev", dev_node.get_inode_num());

    sector_dir bin_sec_dir;
    bin_sec_dir.dirs[0].init(".", bin_node.get_inode_num());
    bin_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir etc_sec_dir;
    etc_sec_dir.dirs[0].init(".", etc_node.get_inode_num());
    etc_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir home_sec_dir;
    home_sec_dir.dirs[0].init(".", home_node.get_inode_num());
    home_sec_dir.dirs[1].init("..", root_node.get_inode_num());
    home_sec_dir.dirs[2].init("tangrui", tangrui_node.get_inode_num());

    sector_dir dev_sec_dir;
    dev_sec_dir.dirs[0].init(".",  dev_node.get_inode_num());
    dev_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir tangrui_sec_dir;
    tangrui_sec_dir.dirs[0].init(".",  tangrui_node.get_inode_num());
    tangrui_sec_dir.dirs[1].init("..", home_node.get_inode_num());

    cout << "3. 目录创建完成" << endl;
    /*
    *  5. 将文件夹对应的扇区写入到磁盘中
    */
    root_sec_dir.write_back_to_disk(my_cache, root_node.get_sec_beg());
    bin_sec_dir.write_back_to_disk(my_cache, bin_node.get_sec_beg());
    etc_sec_dir.write_back_to_disk(my_cache, etc_node.get_sec_beg());
    home_sec_dir.write_back_to_disk(my_cache, home_node.get_sec_beg());
    dev_sec_dir.write_back_to_disk(my_cache, dev_node.get_sec_beg());
    tangrui_sec_dir.write_back_to_disk(my_cache, tangrui_node.get_sec_beg());
    cout << "4.目录创建完成" << endl;
    /*
    *  6. 修改系统当前目录位置为根目录
    */
    cur_dir = root_sec_dir;
    cur_dir_node = root_node;
    cout << "******************************** 初始化磁盘结束 ********************************" << endl;
    return true;
}

// mkdir: 创建文件夹
bool my_fs::make_dir(const char* name) {
    cout << "******************************** 创建新文件夹开始 ********************************" << endl;
    // 1. 创建inode
    Inode new_dir_inode(sp.get_new_inode(), false, 0, sp.get_new_sec());

    // 2. 写回磁盘
    new_dir_inode.write_inode_back_to_disk(my_cache);

    // 3. 建立目录结构
    sector_dir new_sec_dir;
    new_sec_dir.dirs[0].init(".", new_dir_inode.get_inode_num());
    new_sec_dir.dirs[1].init("..", cur_dir_node.get_inode_num());
    new_sec_dir.write_back_to_disk(my_cache, new_dir_inode.get_sec_beg());

    // 4. 当前目录中添加一项
    int flag = false;
    for(int i = 2; i < 15; i++) {
        if(cur_dir.dirs[i].inode_num == 0) {
            cur_dir.dirs[i].init(name, new_dir_inode.get_inode_num());
            flag = true;
            break;
        }
    }
    if(flag) {
        // 5. 将修改的目录写回磁盘
        cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());
    }

    return flag;
    cout << "******************************** 创建新文件夹结束 ********************************" << endl;
}

// touch: 创建文件
bool my_fs::touch(const char* name) {
    cout << "******************************** 创建新文件开始 ********************************" << endl;
    // 1. 创建inode
    Inode new_file_inode(sp.get_new_inode(), true, 1, sp.get_new_sec());
    new_file_inode.write_inode_back_to_disk(my_cache);

    // 2. 当前目录添加一项
    int flag = false;
    for(int i = 2; i < 15; i++) {
        if(cur_dir.dirs[i].inode_num == 0) {
            cur_dir.dirs[i].init(name, new_file_inode.get_inode_num());
            flag = true;
            break;
        }
    }

    if(flag) {
        cur_dir.write_back_to_disk(my_cache, new_file_inode.get_sec_beg());
    }

    return flag;

    cout << "******************************** 创建新文件结束 ********************************" << endl;
}
