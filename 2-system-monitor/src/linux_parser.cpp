#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
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
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float mem_uti = 0.0;
  int mem_total{0}, mem_free{0}, buffers{0};
  string line, key, value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      // std::cout << "key = " << key << ", value = " << value << std::endl;
      if (key == "MemTotal:") {
        // std::cout << "Found memTotal" << std::endl;
        mem_total = stoi(value);
      } else if (key == "MemFree:") {
        // std::cout << "Found memFree" << std::endl;
        mem_free = stoi(value);
      } else if (key == "Buffers:") {
        // std::cout << "Found buffers" << std::endl;
        buffers = stoi(value);
      }
    }

    mem_uti = 1.0 - (mem_free * 1.0 / (mem_total - buffers));
  }

  return mem_uti;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    // std::cout << "uptime line = " << line << std::endl;
    std::istringstream(line) >> uptime;
  }

  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() {
  vector<int> cpu_uti{};

  std::ifstream filesystem(kProcDirectory + kStatFilename);
  if (filesystem.is_open()) {
    string line;
    std::getline(filesystem, line);
    std::istringstream stream(line);
    string value;
    while (stream >> value) {
      if (value == "cpu") continue;
      //std::cout << "read value = " << value << std::endl;
      cpu_uti.push_back(stoi(value));
      value.clear();
    }
  }

  return cpu_uti;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes{0};
  string line, key, value;

  std::ifstream filesystem(kProcDirectory + kStatFilename);
  if (filesystem.is_open()) {
    while (std::getline(filesystem, line)) {
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "processes") {
        total_processes = stoi(value);
      }
    }
  }

  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_processes{0};
  string line, key, value;

  std::ifstream filesystem(kProcDirectory + kStatFilename);
  if (filesystem.is_open()) {
    while (std::getline(filesystem, line)) {
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "procs_running") {
        running_processes = stoi(value);
      }
    }
  }

  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string process_cmd;

  std::ifstream filesystem(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filesystem.is_open()) {
    std::getline(filesystem, process_cmd);
  }

  return process_cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;

  std::ifstream filesystem(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filesystem.is_open()) {
    while (std::getline(filesystem, line)) {
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "VmSize:") {
        return std::to_string(stoi(value) / 1024);
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key, value;

  std::ifstream filesystem(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filesystem.is_open()) {
    while (std::getline(filesystem, line)) {
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }

  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string process_uid = Uid(pid);
  //std::cout << "Process_pid = " << process_uid << std::endl;
  string line;
  string user, password, uid;

  std::ifstream filesystem(kPasswordPath);
  if (filesystem.is_open()) {
    while (std::getline(filesystem, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      stream >> user >> password >> uid;
      //std::cout << "user = " << user << std::endl;
      if (uid == process_uid) {
        return user;
      }
    }
  }

  return string();
}

// I Replaced the UpTime and ProcessUtilization with this functions as both function read same file
vector<string> LinuxParser::ProcessState(int pid) {
  vector<string> process_state{};
  string token;
  std::ifstream filesystem(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filesystem.is_open()) {
    while (std::getline(filesystem, token, ' ')) {
      process_state.push_back(token);
    }
  }
  return process_state;
}
