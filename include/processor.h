#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "system_data_manager.h"

class Processor
{
    public:
        Processor();
        Processor(SystemDataManager* systemDataManager);
        float Utilization();

    // TODO: Declare any necessary private members
    private:
        // These are state variables that change with each call to Utilization()
        SystemDataManager* _systemDataManager;
        long _totalJiffies{0};
        long _idleJiffies{0};
        long _activeJiffies{0};

        float CalculateUtilization(long currentTotalJiffies, long currentActiveJiffies);
        void UpdateCpuUtilization(long currentTotalJiffies, long currentActiveJiffies, long currentIdleJiffies);
};

#endif