#include <vector>

#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
  /* retrun aggregated CPU utilization percentage for the interval since the last
  time this function was called.
   Follow formula at
   https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  */

  using namespace LinuxParser;
  std::vector<long> cpu_data = CpuUtilization();
  long utilized_time = cpu_data[kUser_] + cpu_data[kNice_] +
                       cpu_data[kSystem_] + cpu_data[kIRQ_] +
                       cpu_data[kSoftIRQ_] + cpu_data[kSteal_];
  long total_time = utilized_time + cpu_data[kIdle_] + cpu_data[kIOwait_];
  float cpu_utilization = float(utilized_time - previous_utilized_time) /
                          (total_time - previous_total_time);
  previous_total_time = total_time;
  previous_utilized_time = utilized_time;
  return cpu_utilization;
}