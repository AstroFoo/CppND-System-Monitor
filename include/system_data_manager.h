#ifndef SYSTEM_DATA_MANAGER_H
#define SYSTEM_DATA_MANAGER_H

#include <string>
#include <vector>

#include "process.h"

// --- Base class for system data managers.  Derived classes will implement this for different
//     platforms, file systems or other ways in which system data can be configured
//
class SystemDataManager
{
    public:
        virtual std::string GetOSVersion() = 0;
        virtual std::string GetKernelVersion() = 0;
        virtual float GetMemoryUtilisation() = 0;
        virtual int GetTotalProcesses() = 0;
        virtual int GetRunningProcesses() = 0;
        virtual long GetSystemUptime() = 0;
        virtual std::vector<Process> GetProcesses() = 0;
        virtual long GetCpuTotalJiffies() = 0;
        virtual long GetCpuActiveJiffies() = 0;
        virtual long GetCpuIdleJiffies() = 0;
};

#endif