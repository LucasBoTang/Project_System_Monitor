#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "linux_parser.h"

class Processor {
 public:
  Processor() {
    prev_total.resize(num_cpu);
    prev_idle.resize(num_cpu);
  }
  std::vector<float> Utilization();
 private:
  int const num_cpu = LinuxParser::CpuUtilization().size();
  std::vector<long> prev_total;
  std::vector<long> prev_idle;
};

#endif