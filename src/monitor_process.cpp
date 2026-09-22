#include <string>
#include <vector>
#include <libproc.h>
#include <unordered_map>
#include <chrono>

#include "monitor_process.h"

using namespace std;

constexpr double BYTES_PER_GB = 1024.0 * 1024.0 * 1024.0;

bool process_info(pid_t pid, string& name, int& parent_pid) {
    struct proc_bsdinfo proc;

    int bytes_returned = proc_pidinfo(
        pid,
        PROC_PIDTBSDINFO,
        0,
        &proc,
        PROC_PIDTBSDINFO_SIZE
    );

    if (bytes_returned != PROC_PIDTBSDINFO_SIZE) {
        return false;
    }

    name = string(proc.pbi_name);
    parent_pid = proc.pbi_ppid;

    return true;
}

float process_cpu_usage(pid_t pid) {
    struct rusage_info_v2 info;

    if (proc_pid_rusage(pid, RUSAGE_INFO_V2, (rusage_info_t *)&info) != 0) {
        return 0;
    }

    uint64_t cpu_time = info.ri_user_time + info.ri_system_time;

    static unordered_map<pid_t, uint64_t> previous_cpu;
    static unordered_map<pid_t, chrono::steady_clock::time_point> previous_time;

    auto now = chrono::steady_clock::now();

    if (!previous_cpu.contains(pid)) {
        previous_cpu[pid] = cpu_time;
        previous_time[pid] = now;
        return 0;
    }

    uint64_t cpu_delta = cpu_time - previous_cpu[pid];

    double time_delta =
        chrono::duration<double>(now - previous_time[pid]).count();

    previous_cpu[pid] = cpu_time;
    previous_time[pid] = now;

    double cpu_seconds = cpu_delta / 1'000'000'000.0;

    return cpu_seconds / time_delta;
}

float process_memory_usage(pid_t pid) {
    struct proc_taskinfo info;

    int bytes_returned = proc_pidinfo(
        pid,
        PROC_PIDTASKINFO,
        0,
        &info,
        sizeof(info)
    );

    if (bytes_returned != sizeof(info)) {
        return 0;
    }

    return float(info.pti_resident_size) / BYTES_PER_GB;
}

vector<Process> get_processes() {
    int buffer_size = proc_listallpids(nullptr, 0);

    vector<pid_t> pids(buffer_size / sizeof(pid_t));

    int bytes_used = proc_listallpids(
        pids.data(),
        pids.size() * sizeof(pid_t)
    );

    int pid_count = bytes_used / sizeof(pid_t);

    vector<Process> processes;

    for (int i = 0; i < pid_count; i++) {
        if (pids[i] <= 0) {
            continue;
        }

        Process process;

        process.pid = pids[i];

        if (!process_info(
                pids[i],
                process.name,
                process.parent_pid
            )) {
            continue;
        }

        process.cpu_usage = process_cpu_usage(pids[i]);
        process.memory_usage = process_memory_usage(pids[i]);

        processes.push_back(process);
    }

    return processes;
}

vector<Process> group_processes(const vector<Process>& processes) {
    unordered_map<int, Process> process_by_pid;

    for (const auto& process : processes) {
        process_by_pid[process.pid] = process;
    }

    unordered_map<int, Process> grouped;

    for (const auto& process : processes) {
        int root_pid = process.pid;
        int parent_pid = process.parent_pid;

        while (
            parent_pid > 1 &&
            process_by_pid.contains(parent_pid)
        ) {
            root_pid = parent_pid;
            parent_pid = process_by_pid[parent_pid].parent_pid;
        }

        if (!grouped.contains(root_pid)) {
            Process root = process_by_pid[root_pid];

            root.cpu_usage = 0;
            root.memory_usage = 0;

            grouped[root_pid] = root;
        }

        grouped[root_pid].cpu_usage += process.cpu_usage;
        grouped[root_pid].memory_usage += process.memory_usage;
    }

    vector<Process> result;

    for (const auto& [pid, process] : grouped) {
        result.push_back(process);
    }

    return result;
}