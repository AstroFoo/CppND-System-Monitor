#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int processId, std::string userId,  std::string command, long uptime, long ram, float cpuUtilization);
    int Pid() const;
    std::string User() const;
    std::string Command() const;
    float CpuUtilization() const;
    long Ram() const;
    long int UpTime() const;
    bool operator<(Process const& a) const;
    bool operator>(Process const& a) const;

  private:
    int _id;
    std::string _userId;
    std::string _command;
    long _uptime;
    long _ram;
    float _cpuUtilization;
};

#endif