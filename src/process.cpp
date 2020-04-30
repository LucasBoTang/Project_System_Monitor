#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  long elapsed = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
  long jiffies = LinuxParser::ActiveJiffies(pid);
  long hertz = sysconf(_SC_CLK_TCK);
  cpu_ = (float) jiffies / elapsed / hertz;
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid_); }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid_); }


// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}