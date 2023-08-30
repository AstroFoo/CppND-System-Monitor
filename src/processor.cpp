#include <cmath>
#include <iostream>

#include "processor.h"
#include "system_data_manager.h"

// --- Default constructor required
Processor::Processor(){};

Processor::Processor(SystemDataManager* systemDataManager) : _systemDataManager{systemDataManager}
{
    this->_totalJiffies = _systemDataManager->GetCpuTotalJiffies();
    this->_activeJiffies = _systemDataManager->GetCpuActiveJiffies();
    this->_idleJiffies = _systemDataManager->GetCpuIdleJiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    auto currentTotalJiffies = _systemDataManager->GetCpuTotalJiffies();
    auto currentActiveJiffies = _systemDataManager->GetCpuActiveJiffies();
    auto currentIdleJiffies = _systemDataManager->GetCpuIdleJiffies();

    float utilization = CalculateUtilization(currentTotalJiffies, currentActiveJiffies);
    UpdateCpuUtilization(currentTotalJiffies, currentActiveJiffies, currentIdleJiffies);

    // Add up number of jiffies computed then divide by total number of time that has passed (in jiffies)
    return utilization;
}

float Processor::CalculateUtilization(long currentTotalJiffies, long currentActiveJiffies)
{
    double _dTotalJiffies = std::max(0.0, (double)(currentTotalJiffies - Processor::_totalJiffies));
    double _dActiveJiffies = std::max(0.0, (double)(currentActiveJiffies - Processor::_activeJiffies));

    if (_dTotalJiffies <= 0)
        return 0.0;
    return _dActiveJiffies/_dTotalJiffies;
}

void Processor::UpdateCpuUtilization(long currentTotalJiffies, long currentActiveJiffies, long currentIdleJiffies)
{
    Processor::_totalJiffies = currentTotalJiffies;
    Processor::_activeJiffies = currentActiveJiffies;
    Processor::_idleJiffies = currentIdleJiffies;
}