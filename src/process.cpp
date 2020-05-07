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
  /* Return this process's CPU utilization percentage (betwen 0 and 1) since the last refresh.

  Calculated according to:
  https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  */

  MaybeRefreshCpuAndUptimeData();
  return last_refreshed_cpu_utilization;
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
  /* pull the data again if last_refreshed_time was
   * too long ago. The function will update these variables:
   * 
   * - cpu_data_last_refreshed_time
   * - previous_process_uptime
   * - process_uptime
   * - cpu_utilization
   * - previous_process_utilized_time
   *
   * Since both CpuUtilization and uptime calculation use the same data, we want
   * to avoid repeated data reads and instead centralize here.
   * */

  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now - cpu_data_last_refreshed_time);
  if (duration.count() > kRefreshInterval) {
    cpu_data_last_refreshed_time = now;
    LinuxParser::ProcessCpuUtilizationData cpu_data =
        LinuxParser::ProcessCpuUtilization(pid);

    previous_process_uptime = process_uptime;
    // how long the process has been up: 
    // system uptime (dynamic) - process start time (fixed)
    process_uptime =
        LinuxParser::UpTime() * sysconf(_SC_CLK_TCK) - cpu_data.start_time;

    long process_utilized_time = cpu_data.user_time + cpu_data.system_time +
                                 cpu_data.children_user_time +
                                 cpu_data.children_system_time;
    if (process_uptime - previous_process_uptime != 0){
      last_refreshed_cpu_utilization =
          (float)(process_utilized_time - previous_process_utilized_time) /
          (process_uptime - previous_process_uptime);
      previous_process_utilized_time = process_utilized_time;
    }

  }
}

bool Process::operator<(Process const& a) const {
  return last_refreshed_cpu_utilization < a.last_refreshed_cpu_utilization;
}