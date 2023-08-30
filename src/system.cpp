#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "platform.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_system_data_manager.h"

using namespace Platform;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

System::System(Platform::Platform platform)
{
    // TODO: implement system for different platforms
    switch(platform)
    {
        case Platform::Platform::kLinux:
            System::_systemDataManager = new LinuxSystemDataManager();
            break;
        case Platform::Platform::kWindows:
            throw "Windows not supported; only Linux is currently supported.";
            break;
        case Platform::Platform::kMac:
            throw "Mac not supported; only Linux is currently supported.";
            break;
        default:
            throw "Current platform not supported; only Linux is currently supported.";
            break;
    }

    // Create CPU instance.
    System::_cpu = Processor(System::_systemDataManager);
}

// TODO: Return the system's CPU
Processor& System::Cpu()
{
    return System::_cpu;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    System::_processes.clear();
    System::_processes = _systemDataManager->GetProcesses();
    return System::_processes;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return _systemDataManager->GetKernelVersion(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return _systemDataManager->GetMemoryUtilisation(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return _systemDataManager->GetOSVersion(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return _systemDataManager->GetRunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return _systemDataManager->GetTotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return _systemDataManager->GetSystemUptime(); }
