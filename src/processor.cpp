#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;

// Return the aggregate CPU utilization
vector<float> Processor::Utilization() {
  vector<float> cpu_utils(num_cpu);
  auto total = LinuxParser::Jiffies();
  auto idle = LinuxParser::IdleJiffies();
  for (int i = 0; i < num_cpu; i++) {
    // differentiate
    float totald = total[i] - prev_total[i];
    float idled = idle[i] - prev_idle[i];
    // percentage
    cpu_utils[i] = (totald - idled) / totald;
    // save prev
    prev_total[i] = total[i];
    prev_idle[i] = idle[i];
  }
  return cpu_utils;
}