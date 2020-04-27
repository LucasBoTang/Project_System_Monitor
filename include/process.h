#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
  public:
    Process(int pid);
    int Pid() const;
    std::string User();
    std::string Command();
    float CpuUtilization() const;
    std::string Ram(); 
    long UpTime(); 
    bool operator<(Process const& a) const; 

  private:
   int pid_;
   float cpu_;
};

#endif