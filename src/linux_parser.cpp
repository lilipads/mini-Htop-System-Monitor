#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::string LinuxParser::GetValueFromFile(std::string path, std::string key) {
  /* A helper function that retrieves the value from a file at "path"
    that is formated like key value pairs.

    Assumptions:
    - no spaces in the key name.
    - Key and value are separated by spaces.
    - One key per line.
  */

  std::ifstream filestream(path);
  std::string line;
  string temp, value;

  if (!filestream.is_open()) return value;

  while (std::getline(filestream, line)) {
    if (line.substr(0, key.length()) == key) {
      std::istringstream linestream(line);
      linestream >> temp >> value;
      break;
    }
  }

  return value;
}

string LinuxParser::OperatingSystem() {
  /*parse and retur the OS in pretty name, e.g. Ubuntu 16.04.6 LTS */

  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  /* parse and return the kernel name, e.g. 4.15.0-1055-gcp */

  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  /* read and return the system memory utilization, defined as
     1 - free_memory / total_memory
  */

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  string line, key;
  std::istringstream linestream;
  int total_memory, free_memory;

  if (!filestream.is_open()) return 0;  // default value to handle error

  // first line example - MemTotal:    12341 KB
  std::getline(filestream, line);
  linestream.str(line);
  linestream >> key >> total_memory;

  // second line example - MemFree:    1234KB
  std::getline(filestream, line);
  linestream.str(line);
  linestream >> key >> free_memory;

  return 1.0 - float(free_memory) / total_memory;
}

long LinuxParser::UpTime() {
  /* Read and return the system uptime. */

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::string line;

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string uptime_seconds;
    linestream >> uptime_seconds;
    return std::stol(uptime_seconds);
  }

  return -1;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<long> LinuxParser::CpuUtilization() {
  /* Read and return the raw data of CPU utilization.
   * return: a vector of size 10, representing the jiffies by (user,
   * nice, system, idle, IOWait, irq, softIrq, steal, guest, guestNice)
   */

  const int kReturnSize = 10;
  vector<long> cpu_utlization_data;
  std::string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string value;
    linestream >> value;  // take in the string "cpu"

    for (int i = 0; i < kReturnSize; i++) {
      linestream >> value;
      cpu_utlization_data.emplace_back(std::stoi(value));
    }
  }

  return cpu_utlization_data;
}

int LinuxParser::TotalProcesses() {
  /* Read and return the total number of processes. */

  std::string value =
      GetValueFromFile(kProcDirectory + kStatFilename, "processes");
  return stoi(value);
}

int LinuxParser::RunningProcesses() {
  /* Read and return the number of running processes. */

  std::string value =
      GetValueFromFile(kProcDirectory + kStatFilename, "procs_running");
  return stoi(value);
}

string LinuxParser::Command(int pid) {
  /* Read and return the command associated with a process. */

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  std::string line;
  if (filestream.is_open()) std::getline(filestream, line);
  return line;
}

string LinuxParser::Ram(int pid) {
  /* Read and return the memory used by a process (unit: MB). */

  std::string memory_string_in_kb = GetValueFromFile(
      kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
  // convert from Kb to MB
  return std::to_string(std::stol(memory_string_in_kb) / 1000);
}

string LinuxParser::Uid(int pid) {
  /* Read and return the user ID associated with a process. */

  return GetValueFromFile(kProcDirectory + to_string(pid) + kStatusFilename,
                          "Uid:");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

long LinuxParser::UpTime(int pid) {
  /* Read and return the uptime of a process (unit: second). */

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  const int uptime_stat_index = 21;
  std::string line;
  string uptime_value;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i <= uptime_stat_index; i++) linestream >> uptime_value;
    // uptime is measured in clock ticks. converting to seconds.
    return std::stol(uptime_value) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}