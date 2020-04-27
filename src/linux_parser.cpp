#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// Read and return the Operating System
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the Kernel
string LinuxParser::Kernel() {
  string os, str, kernel;
  string line;
  std::ifstream filestream(kProcDirectory + kVersionFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> os >> str >> kernel;
  }
  return kernel;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float total;
  float free;
  string line;
  string key;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") { total = value; }
      if (key == "MemFree:") { free = value; }
    }
  }
  return (total - free) / total;
};

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int processes;
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = value;
        break;
      }
    }
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int procs_running;
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        procs_running = value;
        break;
      }
    }
  }
  return procs_running;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// Read and return CPU utilization
vector<vector<string>> LinuxParser::CpuUtilization() {
  vector<vector<string>> utils;
  vector<string> util;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key.substr(0, 3) == "cpu") {
        for (int i = 0; i < 10; i++) {
          linestream >> value;
          util.push_back(value);
        }
        utils.push_back(util);
        util = {};
      }
    }
  }
  return utils;
}

// Read and return the number of jiffies for the system
vector<long> LinuxParser::Jiffies() {
  auto utils = LinuxParser::CpuUtilization();
  vector<long> total(utils.size());
  for (int i = 0; i < utils.size(); i++) {
    for (string value : utils[i]) {
      total[i] += stol(value);
    }
  }
  return total;
}

// Read and return the number of idle jiffies for the system
vector<long> LinuxParser::IdleJiffies() {
  auto utils = LinuxParser::CpuUtilization();
  vector<long> idle(utils.size());
  for (int i = 0; i < utils.size(); i++) {
    // idle + iowait
    idle[i] = stol(utils[i][3]) + stol(utils[i][4]);
  }
  return idle;
}

// Read and return the number of active jiffies for the system
// archived
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the pids
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        break;
      }
    }
  }
  return uid; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string user;
  string str;
  string uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> str >> uid;
      if (uid == LinuxParser::Uid(pid)) {
        break;
      }
    }
  }
  return user; 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long utime, stime, cutime, cstime, starttime;
  string line;
  string str;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 18; i++) {
      if (i == 14) {linestream >> utime;}
      else if (i == 15) {linestream >> stime;}
      else if (i == 16) {linestream >> cutime;}
      else if (i == 17) {linestream >> cstime ;}
      else {linestream >> str;}
    }
  }
  return utime + stime + cutime + cstime;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  long ram;
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ram = value;
        break;
      }
    }
  }
  return to_string(ram / 1024); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
  long starttime;
  string line;
  string str;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 23; i++) {
      if (i == 22) {linestream >> starttime;}
      else {linestream >> str;}
    }
  }
  return starttime / sysconf(_SC_CLK_TCK);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd);
  }
  return cmd;
}