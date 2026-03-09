#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int priority; 
};

#endif