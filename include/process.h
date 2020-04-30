#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
  public:
    Process(int pid);
    int Pid() const;
    std::string User() const;
    std::string Command() const;
    float CpuUtilization() const;
    std::string Ram() const; 
    long UpTime() const; 
    bool operator<(Process const& a) const; 

  private:
   int pid_;
   float cpu_;
};

#endif