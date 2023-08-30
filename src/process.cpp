#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

Process::Process(int processId, std::string userId, std::string command, long uptime, long ram, float cpuUtilization) :
  _id{processId}, _userId{userId}, _command{command}, _uptime{uptime}, _ram{ram}, _cpuUtilization{cpuUtilization} {}

int Process::Pid() const { return _id; }
float Process::CpuUtilization() const { return _cpuUtilization; }
std::string Process::Command() const { return Process::_command; }
long int Process::Ram() const { return Process::_ram; }
std::string Process::User() const { return Process::_userId; }
long int Process::UpTime() const { return Process::_uptime; }
bool Process::operator<(Process const& a) const
{
  return _cpuUtilization < a.CpuUtilization();
}
bool Process::operator>(Process const& a) const
{
  return _cpuUtilization > a.CpuUtilization();
}