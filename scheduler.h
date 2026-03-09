#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "process.h"

// Remove `using namespace std;` from here

void inputProcesses(std::vector<Process>& processes);
void printResults(std::vector<Process>& processes);

void fcfs(std::vector<Process> processes);
void sjfNonPreemptive(std::vector<Process> processes);
void sjfPreemptive(std::vector<Process> processes);
void roundRobin(std::vector<Process> processes, int quantum);
void priorityNonPreemptive(std::vector<Process> processes);

#endif