#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  int hour = seconds / 3600;
  int minute = (seconds % 3600) / 60;
  int second = (seconds % 3600) % 60;
  string time = to_string(hour) + ":" + to_string(minute) + ":" + to_string(second);
  return time;
};