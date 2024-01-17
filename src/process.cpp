#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_;}

float Process::CpuUtilization() { cpuUtilization = LinuxParser::CpuUtilization(pid_); return cpuUtilization;} 
// LinuxParser::CpuUtilization(pid_)

string Process::Command() { return LinuxParser::Command(pid_); }

// process's memory utilization
string Process::Ram() { ram = LinuxParser::Ram(pid_); return ram; }

//user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

//age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator< (Process const& a) const { 
    return cpuUtilization > a.cpuUtilization;
    // if (cpuUtilization != a.cpuUtilization) {
    //     return cpuUtilization > a.cpuUtilization;
    // }
    // else {return ram > a.ram ;}  //_pid not public may cause issue.}
}
    
 


