#include <iostream>
#include <algorithm>
#include <queue>
#include <iomanip>
#include "scheduler.h"

using namespace std;

/* ================= INPUT ================= */

void inputProcesses(vector<Process>& processes) {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    processes.resize(n);

    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        cout << "Process " << i+1 << " Arrival Time: ";
        cin >> processes[i].arrival;

        cout << "Process " << i+1 << " Burst Time: ";
        cin >> processes[i].burst;

        cout << "Process " << i+1 << " Priority (Lower = Higher Priority): ";
        cin >> processes[i].priority;

        processes[i].remaining = processes[i].burst;
    }
}

/* ================= RESULT PRINT ================= */

void printResults(vector<Process>& processes) {

    if(processes.empty()) return;

    double totalWT = 0, totalTAT = 0;

    cout << "\n-------------------------------------------------------------\n";
    cout << left << setw(5) << "PID"
         << setw(8) << "AT"
         << setw(8) << "BT"
         << setw(8) << "CT"
         << setw(10) << "TAT"
         << setw(8) << "WT" << endl;

    cout << "-------------------------------------------------------------\n";

    for(auto &p : processes) {
        totalWT += p.waiting;
        totalTAT += p.turnaround;

        cout << left << setw(5) << p.pid
             << setw(8) << p.arrival
             << setw(8) << p.burst
             << setw(8) << p.completion
             << setw(10) << p.turnaround
             << setw(8) << p.waiting << endl;
    }

    cout << "-------------------------------------------------------------\n";
    cout << "Average Waiting Time    : " << totalWT / processes.size() << endl;
    cout << "Average Turnaround Time : " << totalTAT / processes.size() << endl;
}

/* ================= FCFS ================= */

void fcfs(vector<Process> processes) {

    sort(processes.begin(), processes.end(),
         [](Process a, Process b){ return a.arrival < b.arrival; });

    int time = 0;

    cout << "\nGantt Chart:\n|";

    for(auto &p : processes) {
        if(time < p.arrival)
            time = p.arrival;

        cout << " P" << p.pid << " |";

        time += p.burst;

        p.completion = time;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
    }

    cout << endl;

    printResults(processes);
}

/* ============ SJF Non-Preemptive ============ */

void sjfNonPreemptive(vector<Process> processes) {

    int n = processes.size();
    int completed = 0, time = 0;
    vector<bool> done(n, false);

    cout << "\nGantt Chart:\n|";

    while(completed < n) {

        int idx = -1;
        int minBurst = 1e9;

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= time && !done[i]) {
                if(processes[i].burst < minBurst) {
                    minBurst = processes[i].burst;
                    idx = i;
                }
            }
        }

        if(idx != -1) {

            cout << " P" << processes[idx].pid << " |";

            time += processes[idx].burst;

            processes[idx].completion = time;
            processes[idx].turnaround = time - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;

            done[idx] = true;
            completed++;
        }
        else {
            time++;
        }
    }

    cout << endl;

    printResults(processes);
}

/* ============ SJF Preemptive (SRTF) ============ */

void sjfPreemptive(vector<Process> processes) {

    int n = processes.size();
    int completed = 0, time = 0;

    cout << "\nGantt Chart:\n|";

    while(completed < n) {

        int idx = -1;
        int minRemaining = 1e9;

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= time &&
               processes[i].remaining > 0 &&
               processes[i].remaining < minRemaining) {

                minRemaining = processes[i].remaining;
                idx = i;
            }
        }

        if(idx != -1) {

            cout << " P" << processes[idx].pid << " |";

            processes[idx].remaining--;
            time++;

            if(processes[idx].remaining == 0) {
                processes[idx].completion = time;
                processes[idx].turnaround = time - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                completed++;
            }
        }
        else {
            time++;
        }
    }

    cout << endl;

    printResults(processes);
}

/* ================= Round Robin ================= */

void roundRobin(vector<Process> processes, int quantum) {

    queue<int> q;
    int n = processes.size();
    int time = 0, completed = 0;

    vector<bool> inQueue(n, false);

    cout << "\nGantt Chart:\n|";

    while(completed < n) {

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= time &&
               processes[i].remaining > 0 &&
               !inQueue[i]) {

                q.push(i);
                inQueue[i] = true;
            }
        }

        if(q.empty()) {
            time++;
            continue;
        }

        int idx = q.front();
        q.pop();

        cout << " P" << processes[idx].pid << " |";

        int execTime = min(quantum, processes[idx].remaining);
        processes[idx].remaining -= execTime;
        time += execTime;

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= time &&
               processes[i].remaining > 0 &&
               !inQueue[i]) {

                q.push(i);
                inQueue[i] = true;
            }
        }

        if(processes[idx].remaining > 0) {
            q.push(idx);
        }
        else {
            processes[idx].completion = time;
            processes[idx].turnaround = time - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            completed++;
        }
    }

    cout << endl;

    printResults(processes);
}

/* ============ Priority Scheduling (Non-Preemptive) ============ */

void priorityNonPreemptive(vector<Process> processes) {

    int n = processes.size();
    int completed = 0, time = 0;
    vector<bool> done(n, false);

    cout << "\nGantt Chart:\n|";

    while(completed < n) {

        int idx = -1;
        int highestPriority = 1e9;

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= time && !done[i]) {
                if(processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if(idx != -1) {

            cout << " P" << processes[idx].pid << " |";

            time += processes[idx].burst;

            processes[idx].completion = time;
            processes[idx].turnaround = time - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;

            done[idx] = true;
            completed++;
        }
        else {
            time++;
        }
    }

    cout << endl;

    printResults(processes);
}