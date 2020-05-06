#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// helper function
std::string GetValueFromFile(std::string path, std::string key);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// a class to help user look up given uid
class UserLookupService{
  public:
  UserLookupService(){BuildUserLookupTable();};
  std::string User(int uid);

  private:
  void BuildUserLookupTable();
  std::unordered_map<int, std::string> user_lookup_table;
};

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<long> CpuUtilization();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
int Uid(int pid);

struct ProcessCpuUtilizationData{
  /* every thing in the unit of clock ticks. */

  long int user_time = 0;
  long int system_time = 0;
  long int children_user_time = 0;
  long int children_system_time = 0;
  long int start_time = 0;
};

ProcessCpuUtilizationData ProcessCpuUtilization(int pid);
};  // namespace LinuxParser

#endif