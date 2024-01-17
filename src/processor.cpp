#include "processor.h"
#include <string>
using std::string;
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
// float Processor::Utilization() { return(float(00000000000.0));}
float Processor::Utilization() { return float(LinuxParser::CpuUtilization());}

