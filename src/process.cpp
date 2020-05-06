#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

string Process::Command() { return LinuxParser::Command(pid); }

string Process::Ram() { return LinuxParser::Ram(pid); }

string Process::User() { return LinuxParser::User(pid); }

long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }