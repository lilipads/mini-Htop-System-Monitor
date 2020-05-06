#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unordered_map>

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
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  Process(int _pid, LinuxParser::UserLookupService _u)
      : pid(_pid), user_lookup_service(_u){};

 private:
  int pid;
  LinuxParser::UserLookupService user_lookup_service;
};

#endif