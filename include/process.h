#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int p):pid_(p) {}
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  //   static std::vector<Process> processes_ = {};
  float cpuUtilization;
  std::string ram;     

 private:
    int pid_;
    // std::string command;
    // std::string user;                         
    // long int upTime; 

};

#endif
