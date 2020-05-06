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

float Process::CpuUtilization() {
  /* Return this process's CPU utilization.

  Calculated according to:
  https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  */

  MaybeRefreshCpuAndUptimeData();
  long process_utilized_time = cpu_data.user_time + cpu_data.system_time +
                               cpu_data.children_user_time +
                               cpu_data.children_system_time;
  float utilization =
      (float)(process_utilized_time - previous_process_utilized_time) /
      (process_uptime - previous_process_uptime);
  previous_process_utilized_time = process_utilized_time;
  return utilization;
}

string Process::Command() { return command; }

string Process::Ram() { return LinuxParser::Ram(pid); }

string Process::User() { return user; }

long int Process::UpTime() {
  /* Return the process uptime in unit of second. */

  MaybeRefreshCpuAndUptimeData();
  return process_uptime / sysconf(_SC_CLK_TCK);
}

void Process::MaybeRefreshCpuAndUptimeData() {
  /* pull the data again and store in class variable if last_refreshed_time was
   * too long ago.
   *
   * Since both CpuUtilization and uptime calculation use the same data, we want
   * to avoid repeated data reads and instead centralize here.
   * */

  auto now = std::chrono::system_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - last_refreshed_time);
  if (duration.count() > kRefreshInterval) {
    last_refreshed_time = now;
    cpu_data = LinuxParser::ProcessCpuUtilization(pid);

    previous_process_uptime = process_uptime;
    // how long the process has been up: system uptime (dynamic) - process
    // start time (fixed)
    process_uptime =
        LinuxParser::UpTime() * sysconf(_SC_CLK_TCK) - cpu_data.start_time;
  }
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }