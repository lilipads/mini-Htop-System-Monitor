#include <vector>

#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
  /* retrun aggregate CPU utilization.
   Follow formula at cpu_data[LinuxParser::CPUStates.kUser]
   */

  using namespace LinuxParser;
  std::vector<long> cpu_data = CpuUtilization();
  cpu_data[kUser_] -= cpu_data[kGuest_];
  cpu_data[kNice_] -= cpu_data[kGuestNice_];
  cpu_data[kIdle_] += cpu_data[kIOwait_];
  cpu_data[kSystem_] += cpu_data[kIRQ_] + cpu_data[kSoftIRQ_];
  cpu_data[kGuest_] += cpu_data[kGuestNice_];
  long total_time = cpu_data[kUser_] + cpu_data[kNice_] + cpu_data[kSystem_] +
                    cpu_data[kIdle_] + cpu_data[kSteal_] + cpu_data[kGuest_];

  return float(cpu_data[kIdle_]) / total_time;
}