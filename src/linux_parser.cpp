#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// --- Helper methods for this parser
//
vector<string> GetCpuData()
{
  std::string item;
  string line;
  vector<string> cpuData;

  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> item)
      cpuData.push_back(item);
  }
  return cpuData;
}

vector<string> GetProcessStats(int pid)
{
  string line;
  string item;
  vector<string> processStats;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> item)
      processStats.push_back(item);
  }
  return processStats;
}

template<typename T> T LinuxParser::ConvertTicksToSeconds(T timeInTicks)
{
    return timeInTicks/sysconf(_SC_CLK_TCK);
}

// --- Functions to parse system data
// 
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, kernel, version;
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
vector<int> LinuxParser::Pids()
{
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

float LinuxParser::MemoryUtilization()
{
  int memTotal = 0;
  int memFree = 0;
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "MemTotal:")
            memTotal = value;
        else if (key == "MemFree:")
            memFree = value;
      }
    }
  }
  
  // Report fraction of "Total used memory", not including cached memory or buffers
  return float(memTotal - memFree)/float(memTotal);
}

long LinuxParser::UpTime()
{
  long uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }
  return 0;
}

long LinuxParser::ActiveJiffies()
{
  auto cpuData = GetCpuData();
  long userJiffies = std::stol(cpuData[1]);
  long niceJiffies = std::stol(cpuData[2]);
  long systemJiffies = std::stol(cpuData[3]);
  return userJiffies + niceJiffies + systemJiffies;
}

// This is the total system Jiffies from the aggregated CPU statistics in /proc/stat
long LinuxParser::Jiffies()
{
  auto cpuData = GetCpuData();
  long userJiffies = std::stol(cpuData[1]);
  long niceJiffies = std::stol(cpuData[2]);
  long systemJiffies = std::stol(cpuData[3]);
  long idleJiffies = std::stol(cpuData[4]);
  return userJiffies + niceJiffies + systemJiffies + idleJiffies;
}

// This is the total system idle Jiffies from the aggregated CPU statistics in /proc/stat
long LinuxParser::IdleJiffies()
{
  auto cpuData = GetCpuData();
  long idleJiffies = std::stol(cpuData[4]);
  return idleJiffies;
}

int LinuxParser::TotalProcesses()
{
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
            return value;
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses()
{
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
            return value;
      }
    }
  }
  return 0;
}

// --- Functions to parse process data
// 
long LinuxParser::ActiveTime(int pid, bool includeChildProcesses)
{
  vector<string> processStats = GetProcessStats(pid);
  long totaltime = 0;
  if (processStats.size() > 22)
  {
    long utime = std::stol(processStats[13]);
    long stime = std::stol(processStats[14]);
    long cutime = std::stol(processStats[15]);
    long cstime = std::stol(processStats[16]);

    totaltime = utime + stime;
    if (includeChildProcesses)
      totaltime += cutime + cstime;
  }
  return ConvertTicksToSeconds(totaltime);
}

string LinuxParser::Command(int pid)
{
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    return line;
  }
  return string();
}

long LinuxParser::Ram(int pid)
{
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "VmSize:")
            return value;
      }
    }
  }
  return 0;
}

string LinuxParser::Uid(int pid)
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid:")
            return value;
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid)
{
  vector<string> processStats = GetProcessStats(pid);
  if (processStats.size() > 22)
    return ConvertTicksToSeconds(std::stol(processStats[21]));
  else
    return 0;
}

string LinuxParser::User(int pid)
{
  string uid = Uid(pid);
  string line;
  string substring;
  char delimiter = ':';
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      vector<string> splitLine;
      while (std::getline(linestream, substring, delimiter))
      {
        splitLine.push_back(substring);
      }
      if (splitLine[2] == uid)
        return string(splitLine[0]);
    }
  }
  return string();
}