#include <cmath>
#include <vector>

#include "linux_parser.h"
#include "linux_system_data_manager.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include "system_data_manager.h"

template<typename T> T ConvertkbToMb(T ramInkB)
{
    return (T)round(ramInkB/1000.0);
}

float ProcessCpuUtilization(int pid)
{
    // Include child processes
    long starttime = LinuxParser::UpTime(pid);
    long totaltime = LinuxParser::ActiveTime(pid, true);
    long elapsedtime = LinuxParser::UpTime() - starttime;
    if (elapsedtime <= 0L)
        return 0.0;
    return (float)(100.0*totaltime/elapsedtime); // %
}

// --- Use a cache for system info, since it's never going to change
//
std::string LinuxSystemDataManager::GetOSVersion()
{
    if (!LinuxSystemDataManager::_systemInfoRetrieved)
        LinuxSystemDataManager::GetLinuxSystemInfo();
    return LinuxSystemDataManager::_os_version;
}

std::string LinuxSystemDataManager::GetKernelVersion()
{
    if (!LinuxSystemDataManager::_systemInfoRetrieved)
        LinuxSystemDataManager::GetLinuxSystemInfo();
    return LinuxSystemDataManager::_kernel_version;
}

// --- For all other information, let the UI decide when to retrieve data.  Most of these are
//     pass-throughs to the Linux parser, but this can change if the Linux parser is refactored
//     (e.g. implementing serialisation)
//
float LinuxSystemDataManager::GetMemoryUtilisation() { return LinuxParser::MemoryUtilization(); }
int LinuxSystemDataManager::GetTotalProcesses() { return LinuxParser::TotalProcesses(); }
int LinuxSystemDataManager::GetRunningProcesses() { return LinuxParser::RunningProcesses(); }
long LinuxSystemDataManager::GetSystemUptime() { return LinuxParser::UpTime(); }
long LinuxSystemDataManager::GetCpuTotalJiffies() { return LinuxParser::Jiffies(); }
long LinuxSystemDataManager::GetCpuActiveJiffies() { return LinuxParser::ActiveJiffies(); }
long LinuxSystemDataManager::GetCpuIdleJiffies() { return LinuxParser::IdleJiffies(); }

std::vector<Process> LinuxSystemDataManager::GetProcesses()
{
    std::vector<Process> _processes;
    for (auto processId : LinuxParser::Pids())
    {
        std::string userId = LinuxParser::User(processId);
        std::string command = LinuxParser::Command(processId);
        long uptime = LinuxParser::UpTime(processId);
        long ram = ConvertkbToMb(LinuxParser::Ram(processId));
        float cpuUtilization = ProcessCpuUtilization(processId);
        Process process(processId, userId, command, uptime, ram, cpuUtilization);
        _processes.emplace_back(process);
    }
    // Sort the processes in descending order
    std::sort(_processes.begin(), _processes.end(), std::greater<>());
    // Pass a copy of the vector back out to the system container for storage
    return _processes;
}

// --- Private helper methods
//
void LinuxSystemDataManager::GetLinuxSystemInfo()
{
    LinuxSystemDataManager::_os_version = LinuxParser::OperatingSystem();
    LinuxSystemDataManager::_kernel_version = LinuxParser::Kernel();
    LinuxSystemDataManager::_systemInfoRetrieved = true;
}