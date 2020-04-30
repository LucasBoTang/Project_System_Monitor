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
  std::vector<Process>& Processes();
  std::string OperatingSystem() const;
  std::string Kernel() const;
  float MemoryUtilization() const;
  int TotalProcesses() const;
  int RunningProcesses() const;
  long UpTime() const; 

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif