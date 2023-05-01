#include "processor.h"

#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto cpu_values = LinuxParser::CpuUtilization();

  uint64_t currentNonIdle = cpu_values[LinuxParser::CPUStates::kUser_] +
                            cpu_values[LinuxParser::CPUStates::kNice_] +
                            cpu_values[LinuxParser::CPUStates::kSystem_] +
                            cpu_values[LinuxParser::CPUStates::kIRQ_] +
                            cpu_values[LinuxParser::CPUStates::kSoftIRQ_] +
                            cpu_values[LinuxParser::CPUStates::kSteal_];

  uint64_t currentIdle = cpu_values[LinuxParser::CPUStates::kIdle_] +
                         cpu_values[LinuxParser::CPUStates::kIOwait_];

  uint64_t currentTotal = currentIdle + currentNonIdle;

  uint64_t total = currentTotal - prevTotal;
  uint64_t idle = currentIdle - prevIdel;

  float percentage = 0;

  if (total > 0) {
      percentage = ((float)(total - idle) / total);
  }

  return percentage;
}