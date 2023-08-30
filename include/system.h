#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "platform.h"
#include "process.h"
#include "processor.h"
#include "system_data_manager.h"

using namespace Platform;

class System
{
    public:
        System(Platform::Platform platform);
        Processor& Cpu();
        std::vector<Process>& Processes();
        float MemoryUtilization();
        long UpTime();
        int TotalProcesses();
        int RunningProcesses();
        std::string Kernel();
        std::string OperatingSystem();

    private:
        SystemDataManager* _systemDataManager;
        Processor _cpu;
        std::vector<Process> _processes = {};
};

#endif