// this file handles system data and will return such

#include "process.h"
#include "processor.h"
#include "system.h"

using namespace.std

// system's CPU
Processor &System::CPU() {
    return cpu_;
}

// container of system's processes
vector<Process> &System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (unsigned int i = 0; i < pids.size(); i++) {
        Process p = Process(pids[i]);
        processes_.push_back(p);
    }
    // sort processes by CPU usage
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
    return processes_;
}

// return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }