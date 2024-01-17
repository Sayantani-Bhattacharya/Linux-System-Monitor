#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { return cpu_ ; }

vector<Process>& System::Processes() { 
  processes_.clear(); 
  vector<int> pids =  LinuxParser::Pids();
  for (int i: pids){
    Process obj = Process(i);
    obj.User();
    obj.Command();
    obj.CpuUtilization();
    obj.Ram();
    obj.UpTime();
    // obj.cpuUtilization = LinuxParser::CpuUtilization(i);
    processes_.emplace_back(obj);
  }  
  std::sort(processes_.begin(), processes_.end(), std::less<Process>());
  return processes_; 
  }
std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses();  }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }

