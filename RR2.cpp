#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

struct Process {
    string id;
    int servicetime = 0;
    int arrivaltime = 0;
    int priority = 0;
    int start_time = -1;
    int end_time = -1;
    int wait_time = -1;
};

void roundRobinScheduling(const vector<Process>& processes, int quantum) {
    int n = processes.size();
    queue<int> readyQueue;
    vector<int> remainingTime(n, 0);
    vector<int> turnaroundTime(n, 0);
    vector<int> waitingTime(n, 0);
    vector<int> responseTime(n, -1);

    int currentTime = 0;
    int completedProcesses = 0;

    for (int i = 0; i < n; i++) {
        remainingTime[i] = processes[i].servicetime;
    }

    while (completedProcesses < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currentTime && remainingTime[i] > 0) {
                readyQueue.push(i);
                if (responseTime[i] == -1) {
                    responseTime[i] = currentTime - processes[i].arrivaltime;
                }
            }
        }

        if (readyQueue.empty()) {
            currentTime++;
        }
        else {
            int currentProcess = readyQueue.front();
            readyQueue.pop();

            if (remainingTime[currentProcess] > quantum) {
                currentTime += quantum;
                remainingTime[currentProcess] -= quantum;
            }
            else {
                currentTime += remainingTime[currentProcess];
                turnaroundTime[currentProcess] = currentTime - processes[currentProcess].arrivaltime;
                waitingTime[currentProcess] = turnaroundTime[currentProcess] - processes[currentProcess].servicetime;
                remainingTime[currentProcess] = 0;
                completedProcesses++;
            }

            for (int i = 0; i < n; i++) {
                if (i != currentProcess && processes[i].arrivaltime <= currentTime && remainingTime[i] > 0) {
                    readyQueue.push(i);
                }
            }
        }
    }

    // Print results
    cout << "Process\tTurnaround Time\tWaiting Time\tResponse Time\n";
    for (int i = 0; i < n; i++) {
        cout << processes[i].id << "\t" << turnaroundTime[i] << "\t\t" << waitingTime[i] << "\t\t" << responseTime[i] << endl;
    }
}

int main() {
    ifstream infile("Processes.txt");
    vector<Process> processes;

    if (!infile) {
        cout << "Error opening file" << endl;
        return 1;
    }

    while (!infile.eof()) {
        Process p;
        infile >> p.id >> p.servicetime >> p.arrivaltime >> p.priority;
        processes.push_back(p);
    }

    infile.close();
    //quantum =2 
    int quantum = 2;
    roundRobinScheduling(processes, quantum);

    return 0;
}
