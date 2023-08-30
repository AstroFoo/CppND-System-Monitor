#ifndef LINUX_SYSTEM_DATA_MANAGER_H
#define LINUX_SYSTEM_DATA_MANAGER_H

#include <string>
#include <vector>

#include "system_data_manager.h"
#include "process.h"
#include "processor.h"

class LinuxSystemDataManager : public SystemDataManager
{
    public:
        std::string GetOSVersion() override;
        std::string GetKernelVersion() override;
        float GetMemoryUtilisation() override;
        int GetTotalProcesses() override;
        int GetRunningProcesses() override;
        long GetSystemUptime() override;
        std::vector<Process> GetProcesses() override;
        long GetCpuTotalJiffies() override;
        long GetCpuActiveJiffies() override;
        long GetCpuIdleJiffies() override;

    private:
        bool _systemInfoRetrieved = false;

        std::string _os_version;
        std::string _kernel_version;

        void GetLinuxSystemInfo();
        void GetLinuxProcesses();
};

#endif