// this file parses linux os metadata

// dirent.h is a directory stream
// unistd.h is a file that gives access to POSIX OS API like that for linux
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>

#include "linux_parser.h"

using namespace.std;

// strings will be split into a vector of strings by spaces
vector<string> splitString(string s) {
    std::stringstream stringstream(s);
    std::string split;
    vector<string> result;

    // stringstream is a class that permits insertion, extraction, and other operations
    // handles parsing as opposed to regex (regular expressions)
    while(stringstream >> split) {
        result.push_back(split);
    }
    return result;
}

// reading in linux metadata
string LinuxParser::OperatingSystem() {
    string line, value, key;
    std::ifstream filestream(kOSPath);

    // while the file for os is open
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while(linestream >> key >> value) {
                if(key == "PRETTY_NAME") {
                    std::replace(value.begin(), value.end(), '_', ' ')
                    return value;
                }
            }
        }
    }
    return value;
}

// reading in linux kernel metadata
string LinuxParser::Kernel() {
    string os, version, kernel, line;
    // kproc stands for kernel process
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if(stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

// reading in filesystem's running tasks' pids (process ID number used by os kernels to identify active processes)
vector<int> LinuxParser::Pids() {
    vecotr<int> pids;
    DIR *directory = opendir(kProcDirectory.c_str());
    struct dirent *file;
    // while the directory is not null
    while((file = readdir(directory)) != nullptr) {
        // if the file is a directory
        if(file->d_type == DT_DIR) {
            // seeing if every character of the file name is a digit
            string filename(file->d_name);
            if(std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// reading and return system memory utilization in linux
// return the total memory, available memory, and overall utilization
float LinuxParser::MemoryUtilization() {
    float total_mem, available_mem;
    string line, value, key;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while(linestream >> key >> value) {
                if(key == "Memory Total:") {
                    total_mem = std::stof(value);
                }
                else if(key == "Memory Available:") {
                    available_mem = std::stof(value);
                }
            }
        }
    }
    if(total_mem == 0.0) {
        return 0.0;
    }

    // memory utilization formula is total - (free memory, buffers, cache mem) / total
    return (total_mem - available_mem) / total_mem;
}

// reading and returning system uptime (how long system has been running)
long LinuxParser::UpTime() {
    string uptime, line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if(stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        // bringing in uptime from linestream
        linestream >> uptime;
    }
    return std::stol(uptime);
}

// reading and returning CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line, key;
    string usertime, nicetime, systemtime, idletime, iowait, irq, softirq, steal;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while(linestream >> key >> usertime >> nicetime >> systemtime >> idletime >> iowait >> irq >> softirq >> steal) {
                if(key == "cpu") {
                    return {usertime, nicetime, systemtime, idletime, iowait, irq, softirq, steal}
                }
            }
        }
    }
    return {};
}

// reading and returning total number of running processes
int LinuxParser::RunningProcesses() {
    string line, value, key;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while(linestream >> key >> value) {
                if(key == "procs_running") {
                    return std:: stoi(value);
                }
            }
        }
    }
    return 0;
}

// reading and returning the memory used by a process
string LinuxParser::Ram(int pid) {
    string line, value, key;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std:: istringstream linestream(line);
            while(lienstream >> key >> value) {
                // memory ussage to be returned in megabytes
                if(key == "VmSize:") {
                    return std::to_string(int(std::stod(value)/1000));
                }
            }
        }
    }
    return string();
}

// reading a returning the user associated with a process
string LinuxParser::User(int pid) {
    // retrieving UID for give PID
    string uid = Uid(pid);

    string line, name, value, x;
    std::ifstream filestream(kPasswordPath);
    if(filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            // name of the user will be returned if UID matches the UID from the PID
            while(linestream >> name >> x >> value) {
                if(value == uid) {
                    return name;
                }
            }
        }
    }
    return string();
}