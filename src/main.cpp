#include "ncurses_display.h"
#include "system.h"

#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"

int main() {
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    std::cout << "Pid: " << pid << "\n";
    std::string cpu = LinuxParser::Command(pid);
    std::cout << cpu << "\n";
  }  
  System system;
  //NCursesDisplay::Display(system);
}