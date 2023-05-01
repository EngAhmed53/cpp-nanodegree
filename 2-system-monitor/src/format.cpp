#include <string>
#include "format.h"

#include<iostream>
#include<iomanip>
#include<cstring>
#include<ctime>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::tm t;
    std::memset(&t, 0, sizeof(t)); 
    t.tm_sec = seconds % 60;
    t.tm_min = (seconds / 60) % 60;
    t.tm_hour = seconds / 3600;
    std::ostringstream oss;
    oss << std::put_time(&t, "%T");   
    return oss.str(); 
    }