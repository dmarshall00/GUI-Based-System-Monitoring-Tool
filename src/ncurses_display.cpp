#include <chrono>
#include <curses.h>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "ncurses_display.h"
#include "system.h>

using std::string;
using std::to_strong;

//visualize what's happening with a progress bar
std::string NCursesDisplay::ProgressBar(float percent){
  std::string result{"0%};
  int size{50};
  float bars{percent * size};

    //loop through the progress bar
  for(int i{0}; i < size; ++i){
    result += i <= i <= bars ? '|' : ' '; //add a pipe for each iteraton if i's position is <= bars value.
                                           // If it's not,add a space
  }
  
  string display{to_string(percent * 100).substr(0, 4)};  //represent the percent value as string
  if (percent < 0.1 || percent == 1.0){
    display = " " + to_string(percent * 100).substr(0, 3); //update display if condition is met
  }
  return result + " " + display + "/100%";
}

//create a text based UI to display system info
void NCursesDisplay::DisplaySystem(System &system, WINDOW *window) {
  //how the UI will be formatted 
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());//dsplays info of given parameters
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str()); //using progress bar to check CPU and memory usage
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

/*void NCursesDisplay::DisplayProcesses(std::vector<Process> &processes,
                                      WINDOW *window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{16};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(2));
  
  // how the information will look in the window
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));

  //go through each index in the vector and display the pid, user, cpu, ram, and up time
  for (int i = 0; i < n; ++i) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].getCpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}*/

void NCursesDisplay::Display(System &system) {
  initscr();     // start ncurses
  noecho();      // do not print input values
  cbreak();      // terminate ncurses on ctrl + c
  start_color(); // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW *system_window = newwin(9, x_max - 1, 0, 0); //creates windows for process and system information
 // WINDOW *process_window =
     // newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {  //infinite loop to constantly display the system
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);   //box encapsulates both windows to show seperation in UI
    //box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    //DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window); //refresh both windows to display updated information
   // wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));  //pause for 1 sec
  }
  endwin();
}
