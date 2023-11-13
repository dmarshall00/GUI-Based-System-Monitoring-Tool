//process header file

#ifndef PROCESS_H //check if PROCESS_H has been defined
#define PROCESS_H  //macro to define PROCESS_H

#include <string>

//create class Process 
class Process {
public:
  //each member function returns the variable associated with it
  Process(int PID);
  int Pid() const;  
  std::string User();
  std::string Command();
  void updateCpuUtilization();  
  float getCpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;


private:  //get values
  int pid;
  float cpu_utilization{0.0};
};

#endif
