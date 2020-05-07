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
  std::string User();   
  std::string Command();
  std::string Ram(); 
  float CpuUtilization(); 
  long int UpTime();
  bool operator<(Process const& a) const;
  Process(int _pid, std::string _u) : pid(_pid), user(_u) {
    command = LinuxParser::Command(pid);
    MaybeRefreshCpuAndUptimeData();
  };
  float last_refreshed_cpu_utilization = 0.0;

 private:
  // fixed attributes
  int pid;
  std::string command;
  std::string user;
  
  void MaybeRefreshCpuAndUptimeData();
  static const int kRefreshInterval = 400;  // unit: millisecond
  std::chrono::system_clock::time_point cpu_data_last_refreshed_time;
  // dynamic attributes
  
  long int process_uptime = 0;  // unit: clock ticks
  long int previous_process_uptime = 0;  // unit: clock ticks
  long int previous_process_utilized_time = 0;
};

#endif