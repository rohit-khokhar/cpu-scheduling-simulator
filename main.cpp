#include <iostream>
#include <vector>
#include "scheduler.h"

using namespace std;

int main() {

    vector<Process> processes;
    inputProcesses(processes);

    int choice;

    cout << "\nSelect Scheduling Algorithm\n";
    cout << "1. FCFS\n";
    cout << "2. SJF (Non-Preemptive)\n";
    cout << "3. SJF (Preemptive - SRTF)\n";
    cout << "4. Round Robin\n";
    cout << "5. Priority (Non-Preemptive)\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch(choice) {
        case 1:
            fcfs(processes);
            break;

        case 2:
            sjfNonPreemptive(processes);
            break;

        case 3:
            sjfPreemptive(processes);
            break;

        case 4: {
            int quantum;
            cout << "Enter Time Quantum: ";
            cin >> quantum;
            roundRobin(processes, quantum);
            break;
        }

        case 5:
            priorityNonPreemptive(processes);
            break;

        default:
            cout << "Invalid choice\n";
    }

    return 0;
}