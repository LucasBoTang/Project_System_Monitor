#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  std::string OperatingSystem();
  std::string Kernel();
  float MemoryUtilization();
  int TotalProcesses();
  int RunningProcesses();
  long UpTime(); 

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif