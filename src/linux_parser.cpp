#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

long jiffies;
long activejiffies;
long idlejiffies;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  // vector<int> pids;
  LinuxParser::pids.clear();
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        LinuxParser::pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids; 
}

// void LinuxParser::ProcessParser(){
//   for (int i: LinuxParser::Pids()){
//     // std::cout<<i<<std::endl;
//     Process obj(i);
    
//     LinuxParser::processes_.emplace_back(obj);
//   }  
// }

// void LinuxParser::ProcessParser(){
//   Process obj(1);
//   LinuxParser::check.push_back(obj);
//   // std::cout<<"Process made";    
// }

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {   
  string line;
  string key;
  float value;
  
  float value1,value2,value3,value4;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);  
  if (filestream.is_open()) {  
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);    
      while (linestream >> key >> value) {        
        if (key == "MemTotal") { value1 = value;}
        else if (key == "MemFree") { value2 = value;}
        // else if (key == "MemAvailable") { value3 = value;}
        // else if (key == "Buffers") { value4 = value;}        
      }
    }
  }  
  // return (value1-value2-value3-value4);
  return (float(value1-value2)/float(value1));  
}


long LinuxParser::UpTime() { 
  string line;
  int value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;  
 }
 
long LinuxParser::Jiffies() { 
  string line;
  string key;
  std::vector< string > arr;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream>>key;
    for (int i=0; i <10; i++){
      linestream>>key;
      arr.push_back(key);
    }
    // while (std::getline(filestream, line)) {
    //   std::istringstream linestream(line);
    //   string temp;
    //   string found;
    //   while (!linestream.eof()){
    //     linestream>>temp;
    //     if (std::stringstream(temp)>> found)
    //       {arr.emplace_back(std::stoi(found));}
    //   }
    //   temp ="";        
    // }
  }
  long jiffies{0};

  for(string a:arr)
    jiffies += std::stol(a);
  // long jiffies = arr[1]+arr[2]+arr[0]+arr[3]+arr[4]+ arr[5]+arr[6]+arr[7]+arr[8]+arr[9];
  return (jiffies);
}

long LinuxParser::ActiveJiffies() { 

  string line;
  string key;
  std::vector< string > arr;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream>>key;
    for (int i=0; i <10; i++){
      linestream>>key;
      arr.push_back(key);
    }
  }
  long jiffies{0};

  for(string a:arr)
    jiffies += std::stol(a);


  long idlejiffies = std::stol(arr[3]) + std::stol(arr[4]);
  long  activejiffies = jiffies - idlejiffies;
  return (activejiffies);
  }

long LinuxParser::IdleJiffies() {return ( LinuxParser::Jiffies() -  LinuxParser::ActiveJiffies()  )   ; }

long LinuxParser::CpuUtilization() {return (100 * (LinuxParser::ActiveJiffies())/(LinuxParser::Jiffies()));}


int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") 
          // return value;
          // return ( int( 100* LinuxParser::ActiveJiffies()  / LinuxParser::Jiffies())   );
          return value;

        }
      }
    }
  return value;
  // return (  static_cast<float>(LinuxParser::ActiveJiffies() ) /  static_cast<float>(LinuxParser::Jiffies() ) );
  // return (90);
  }

int LinuxParser::RunningProcesses() {   
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return (value);
  } 

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      return (line);
    }
  }
  return (line);
  } 

long LinuxParser::CpuUtilization(int pid) {
  int act;
  string line;
  string a;
  string x,utime,stime,cutime,cstime,total_time,seconds ;
  std::ifstream filestream(kProcDirectory+ std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for ( int i = 1; i < 14; i++){
      linestream>> x;

    }
    linestream >>utime>>stime>>cutime>>cstime;
    act = std::atol(utime.c_str()) +  std::atol(stime.c_str()) +  std::atol(cutime.c_str()) +  std::atol(cstime.c_str()) ;
    return (100 * (act) / (LinuxParser::ActiveJiffies()));

  // total_time = utime+stime+cutime+cstime;
  // seconds = LinuxParser::UpTime(1) - x;
  // act = std::atol(utime.c_str()) +  std::atol(stime.c_str()) ;
  // return ( act / LinuxParser::ActiveJiffies()  );
  // return ( 100 * (total_time/seconds) );

  // }
  // return ( act / LinuxParser::ActiveJiffies()  );
  // return ( 100 * (total_time/ seconds ) ) ; 
}
return (100 * ( act / LinuxParser::ActiveJiffies() ));

}


// TODO: Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+ std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      // std::replace(line.begin(), line.end(), ' ', '_');      
      // std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          // std::replace(value.begin(), value.end(), '_', ' ');
          return (std::to_string(value/1024));
        }
      }
    }
  }
  return ( std::to_string(value/1024));}
  


string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory+ std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      // std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          // std::replace(value.begin(), value.end(), '_', ' ');
          return std::to_string(value);
        }
      }
    }
  }
  return std::to_string(value);}
  

// The user associated with a process

string LinuxParser::User(int pid) { 
  string line;
  string user;
  string x;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value) {
        if (value == LinuxParser::Uid(pid)) {
          std::replace(user.begin(), user.end(), '_', '-');
          return user;
        }
      }
    }
  }

  return user; 
}

long LinuxParser::UpTime(int pid) { 
  string line;
  string a;
  int x;
  int upTimePid;
  std::ifstream filestream(kProcDirectory+ std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >>x>>a>>a>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x>>x) {
        int upTimePid = UpTime() - x/sysconf(_SC_CLK_TCK);
        return upTimePid;        
      }
    }
  }
  return upTimePid; 
}


//CHECK

// int LinuxParser::TotalProcesses() { 

//   string line;
//   string key;
//   std::vector< string > arr;
//   std::ifstream filestream(kProcDirectory + kStatFilename);
//   if (filestream.is_open()) {
//     std::getline(filestream,line);
//     std::istringstream linestream(line);
//     linestream>>key;
//     for (int i=0; i <10; i++){
//       linestream>>key;
//       arr.push_back(key);
//     }
//   }
//   int jiffies{0};

//   for(string a:arr)
//     jiffies += std::stoi(a);


//   long idlejiffies = std::stol(arr[3]) + std::stol(arr[4]);
//   int activejiffies = jiffies - idlejiffies;
//   return (jiffies);
// }
   





  

