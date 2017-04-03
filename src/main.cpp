#include <iostream>
#include "../include/control.h"
using namespace std;


int main() {
  my_fs *file_system = new my_fs;

  file_system->run();

  delete file_system;
  file_system = NULL;
  return 0;
}
