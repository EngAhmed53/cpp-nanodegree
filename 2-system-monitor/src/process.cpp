#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::string;
using std::to_string;
using std::vector;

using namespace LinuxParser;

Process::Process(int pid): pid_(pid), user_(LinuxParser::User(pid)), command_(LinuxParser::Command(pid)),
      ram_(LinuxParser::Ram(pid)) {
  auto process_state = LinuxParser::ProcessState(pid);
  uptime_ =
      LinuxParser::UpTime() - (stol(process_state[21]) / sysconf(_SC_CLK_TCK));
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  auto process_state = LinuxParser::ProcessState(pid_);

  long utime = stol(process_state[13]);
  long stime = stol(process_state[14]);
  long cutime = stol(process_state[15]);
  long cstime = stol(process_state[16]);

  long total_time = utime + stime + cutime + cstime;

  long start_time = LinuxParser::UpTime() -
                    (stol((process_state[21])) * 1.0 / sysconf(_SC_CLK_TCK));

  return static_cast<float>((total_time * 1.0 / sysconf(_SC_CLK_TCK)) /
                            start_time);
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}