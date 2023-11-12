// file handles time formatting for the monitoring tool and generally accounts for time management
// icluding format.h header file

#include <string>
#include "format.h"
using namespace.std

// changing seconds integers into a string with the hh:mm:ss format
// function of type string
string Format::TimePassed(long seconds) {
    long int minutes = seconds / 60; // defining minutes
    long int hours = minutes / 60; // defining hours

    // new variables for metrics
    string hours1, minutes1, seconds1;

    // 10 because 0-9 for numbers in time

    // converting metrics from ints to strings
    if(hours < 10) {
        hours1 = "0" + std::to_string(hours);
    }
    else {
        hours1 = std::to_string(hours)
    }

    if((minutes % 60) < 10) {
        minutes1 = "0" + std::to_string(minutes % 60);
    }
    else {
        minutes1 = std::to_string(minutes % 60);
    }

    if((seconds % 60) < 10) {
        seconds1 = "0" + std::to_string(seconds % 60);
    }
    else {
        seconds1 = std::to_string(seconds % 60);
    }

    // after defining hours, minutes, and seconds return how they should be formatted
    return hours1 + ":" + minutes1 + ":" + seconds1;
}
