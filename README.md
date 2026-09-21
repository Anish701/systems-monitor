# Systems Monitor

A cross-platform system and process monitor written in C++.

SysMon provides live information about CPU usage, memory usage, and running processes directly in the terminal. The project will initially support macOS, with Windows support added through a separate platform-specific backend.

The goal is to keep the core application platform-independent while isolating OS-specific system calls behind a common C++ interface.

## Architecture

```text
                    ┌──────────────────────┐
                    │       main.cpp       │
                    │  application loop    │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │    SystemMonitor     │
                    │                     │
                    │ getCpuUsage()       │
                    │ getMemoryUsage()    │
                    │ getProcesses()      │
                    └──────────┬───────────┘
                               │
                 platform-specific implementation
                               │
                 ┌─────────────┴─────────────┐
                 ▼                           ▼
        ┌─────────────────┐         ┌─────────────────┐
        │ MacSystemMonitor│         │ WinSystemMonitor│
        │                 │         │                 │
        │ Mach            │         │ Windows API     │
        │ libproc         │         │ Win32 / PDH     │
        │ sysctl          │         │                 │
        └────────┬────────┘         └────────┬────────┘
                 │                           │
                 └─────────────┬─────────────┘
                               ▼
                       Normalized C++ data
                               │
                  ┌────────────┴────────────┐
                  ▼                         ▼
           SystemStats                ProcessInfo
           CPU / memory          PID / name / CPU / RAM
                  │                         │
                  └────────────┬────────────┘
                               ▼
                    ┌──────────────────────┐
                    │     Terminal UI      │
                    │                     │
                    │ CPU  █████░ 42%     │
                    │ RAM  ██████ 61%     │
                    │                     │
                    │ PID  NAME CPU  MEM   │
                    └──────────────────────┘
```

Only the platform-specific monitor implementation should know which operating system is running. The process models, terminal UI, application logic, and other shared functionality remain standard C++.

## Project Structure

```text
sysmon/
├── Makefile
├── README.md
└── src/
    ├── main.cpp
    │
    ├── monitor/
    │   ├── system_monitor.h
    │   ├── system_monitor_mac.cpp
    │   └── system_monitor_windows.cpp
    │
    ├── process/
    │   ├── process_info.h
    │   └── process_manager.cpp
    │
    └── ui/
        ├── terminal_ui.h
        └── terminal_ui.cpp
```

The initial implementation will be much smaller and grow toward this structure as features are added.

## Platform Abstraction

The shared interface exposes the information needed by the application:

```cpp
class SystemMonitor {
public:
    virtual double getCpuUsage() = 0;
    virtual MemoryStats getMemoryUsage() = 0;
    virtual std::vector<ProcessInfo> getProcesses() = 0;

    virtual ~SystemMonitor() = default;
};
```

macOS provides its own implementation:

```cpp
class MacSystemMonitor : public SystemMonitor {
public:
    double getCpuUsage() override;
    MemoryStats getMemoryUsage() override;
    std::vector<ProcessInfo> getProcesses() override;
};
```

Windows will eventually implement the same interface:

```cpp
class WindowsSystemMonitor : public SystemMonitor {
public:
    double getCpuUsage() override;
    MemoryStats getMemoryUsage() override;
    std::vector<ProcessInfo> getProcesses() override;
};
```

The rest of the application can therefore work with a `SystemMonitor` without knowing which operating system provided the data.

## Development Plan

### Phase 1 — CPU Monitoring

Implement basic system-wide CPU monitoring on macOS.

Initial goal:

```text
$ ./sysmon
CPU Usage: 17.3%
```

This introduces macOS system APIs and CPU accounting.

### Phase 2 — Memory Monitoring

Add system memory statistics.

```text
CPU: 23.7%
RAM: 11.4 GB / 24.0 GB
```

Track values such as total, used, and available memory.

### Phase 3 — Process Monitoring

Enumerate running processes and collect information about each process.

```text
PID      NAME                  CPU       MEMORY
------------------------------------------------
8392     Google Chrome         17.2%     1.31 GB
4281     Cursor                 8.4%     812 MB
7192     Spotify                2.3%     271 MB
```

Process information will eventually include:

* PID
* Process name
* CPU usage
* Memory usage
* Thread count
* Uptime
* Executable path

### Phase 4 — Live Terminal Dashboard

Turn the one-time statistics into a continuously updating monitor.

```text
collect statistics
        ↓
render terminal
        ↓
wait
        ↓
collect statistics
        ↓
render terminal
        ↓
...
```

The terminal should update in place instead of continuously printing new output.

### Phase 5 — Interactive Process Manager

Add keyboard controls for interacting with the process list.

```text
↑ / ↓     Select process
c         Sort by CPU usage
m         Sort by memory usage
/         Search processes
Enter     Inspect selected process
q         Quit
```

Process termination may later be added with an explicit confirmation step.

### Phase 6 — Process Detail View

Allow inspection of an individual process.

```text
Google Chrome

PID:          8392
CPU:          17.2%
Memory:       1.31 GB
Threads:      38
Uptime:       3h 42m
Executable:   /Applications/Google Chrome.app/...
```

### Phase 7 — Historical Statistics

Store recent measurements to display CPU and memory history.

```text
CPU

100% │
 75% │        ╭─╮
 50% │    ╭───╯ ╰╮
 25% │────╯       ╰─────
  0% └──────────────────
        last 60 seconds
```

A simple container such as `std::deque` can hold a rolling history of measurements.

### Phase 8 — Windows Support

Implement a Windows backend using the appropriate Windows system APIs.

```text
                 SystemMonitor
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
      MacSystemMonitor    WindowsSystemMonitor
             │                   │
     Mach / libproc       Windows APIs
             │                   │
             └─────────┬─────────┘
                       ▼
                  Shared C++
```

The build system will compile the appropriate implementation for the current platform.

Shared code such as the terminal UI, process models, sorting, history, and application logic should require little or no modification.

## Initial Milestone

Do not build the entire architecture immediately.

The first version only needs:

```text
sysmon/
├── Makefile
├── README.md
└── src/
    ├── main.cpp
    └── monitor/
        ├── system_monitor.h
        └── system_monitor_mac.cpp
```

The first milestone is simply:

```text
$ ./sysmon
CPU Usage: 17.3%
```

Once CPU monitoring works correctly, additional functionality can be added incrementally.

## Concepts Covered

This project is intended to provide practical experience with:

### C++

* Classes and interfaces
* RAII and resource ownership
* Smart pointers
* STL containers
* Algorithms and sorting
* Error handling
* Cross-platform abstractions

### Operating Systems

* Processes
* Threads
* CPU accounting
* Memory management
* Process IDs
* System statistics
* OS-level APIs

### Systems Programming

* Calling C system APIs from C++
* Platform-specific implementations
* Polling
* Resource management
* Performance measurement
* Cross-platform software design
* Concurrency

## Platforms

Initial:

* macOS

Planned:

* Windows

The project is written in C/C++ and does not require Objective-C or Swift.
