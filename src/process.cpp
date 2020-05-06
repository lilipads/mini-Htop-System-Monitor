#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

string Process::Command() { return LinuxParser::Command(pid); }

string Process::Ram() { return LinuxParser::Ram(pid); }

string Process::User() {
  int uid = LinuxParser::Uid(pid);
  return user_lookup_service.User(uid);
}

long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }