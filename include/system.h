#ifndef SYSTEM_H
#define SYSTEM_H // define macro SYSTEM_H if not found previously

#include <string>
#include <vector>

//include header files
#include "process.h"
#include "processor.h"

//create system class for computers information
class System {
public:   //member functions to get system info
  Processor &Cpu();
  std::vector<Process> &Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

private:
  // Composition: System "has a" Processor called cpu
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif
