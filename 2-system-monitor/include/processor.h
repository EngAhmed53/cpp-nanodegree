#ifndef PROCESSOR_H
#define PROCESSOR_H

#include<iostream>
#include<iomanip>
#include<cstring>
#include<ctime>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prevTotal{0};
  long prevIdel{0};
};

#endif