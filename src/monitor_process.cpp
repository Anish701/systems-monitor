#include <string>
#include <vector>
#include <libproc.h>
#include <unordered_map>
#include <unistd.h>

using namespace std;

constexpr double BYTES_PER_GB = 1024.0 * 1024.0 * 1024.0;

struct Process {
    int pid;
    string name;
    float cpu_usage;
    float memory_usage;
};

std::string process_name(pid_t pid) {
    struct proc_bsdinfo proc;
    
    int bytes_returned = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
    
    if (bytes_returned == PROC_PIDTBSDINFO_SIZE) {
        return std::string(proc.pbi_name);
    }
    
    return "";
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

    for(int i = 0; i < pid_count; i++) {
        Process process;
        process.pid = pids[i];
        process.name = process_name(pids[i]);

        process.cpu_usage = process_cpu_usage(pids[i]);
        process.memory_usage = process_memory_usage(pids[i]);
        
        processes.push_back(process);
    }

    return processes;
}