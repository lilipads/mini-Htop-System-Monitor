#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>
#include <chrono>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

class Process {
 public:
  int Pid();
  std::string User();     // fixed for a given process
  std::string Command();  // fixed for a given process

  std::string Ram();  // dynamic

  float CpuUtilization();  // dynamic
  long int UpTime();       // dynamic. unit: second
  bool operator<(Process const& a) const;
  Process(int _pid, std::string _u) : pid(_pid), user(_u) {
    command = LinuxParser::Command(pid);
  };

 private:
  int pid;
  long int process_uptime = 0;  // unit: clock ticks
  long int previous_process_uptime = 0;  // unit: clock ticks
  long int previous_process_utilized_time = 0;
  static const int kRefreshInterval = 800;  // unit: millisecond
  std::string command;
  std::string user;
  std::chrono::system_clock::time_point last_refreshed_time;
  void MaybeRefreshCpuAndUptimeData();
  LinuxParser::ProcessCpuUtilizationData cpu_data;
};

#endif