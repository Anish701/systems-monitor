#include <mach/vm_statistics.h>
#include <mach/mach.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>

float memory_usage() {
    vm_size_t page_size;
    vm_statistics64_data_t vm_stats;

    mach_port_t mach_port = mach_host_self();
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count))
    {
        uint64_t free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

        uint64_t used_memory = ((int64_t)vm_stats.active_count +
                                (int64_t)vm_stats.inactive_count +
                                (int64_t)vm_stats.wire_count) * (int64_t)page_size;
        
        return float(used_memory) / float(used_memory + free_memory);
    }

    return -1;
}

float cpu_usage() {
    static uint64_t prev_total = 0;
    static uint64_t prev_idle = 0;

    natural_t cpu_count;
    processor_info_array_t cpu_info;
    mach_msg_type_number_t cpu_info_count;

    kern_return_t result = host_processor_info(
        mach_host_self(),
        PROCESSOR_CPU_LOAD_INFO,
        &cpu_count,
        &cpu_info,
        &cpu_info_count
    );

    if (result != KERN_SUCCESS) {
        return -1;
    }

    uint64_t total = 0;
    uint64_t idle = 0;

    for (natural_t i = 0; i < cpu_count; ++i) {
        uint64_t user =
            cpu_info[CPU_STATE_MAX * i + CPU_STATE_USER];

        uint64_t system =
            cpu_info[CPU_STATE_MAX * i + CPU_STATE_SYSTEM];

        uint64_t nice =
            cpu_info[CPU_STATE_MAX * i + CPU_STATE_NICE];

        uint64_t idle_ticks =
            cpu_info[CPU_STATE_MAX * i + CPU_STATE_IDLE];

        total += user + system + nice + idle_ticks;
        idle += idle_ticks;
    }

    vm_deallocate(
        mach_task_self(),
        reinterpret_cast<vm_address_t>(cpu_info),
        cpu_info_count * sizeof(integer_t)
    );

    if (prev_total == 0) {
        prev_total = total;
        prev_idle = idle;
        return 0.0f;
    }

    uint64_t total_delta = total - prev_total;
    uint64_t idle_delta = idle - prev_idle;

    prev_total = total;
    prev_idle = idle;

    if (total_delta == 0) {
        return 0.0f;
    }

    return float(total_delta - idle_delta) / float(total_delta);
}
