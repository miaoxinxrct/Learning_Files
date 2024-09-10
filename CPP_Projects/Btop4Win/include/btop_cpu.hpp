#ifndef BTOP_CPU_HPP
#define BTOP_CPU_HPP

#include <string>
#include <vector>
#include <tuple>
#include <atomic>

#include "robin_hood.h"

using std::string,std::vector,std::tuple,std::atomic;

namespace Cpu{
    extern string box;
    extern int x,y,width,height,min_width,min_height;
    extern bool shown,redraw,got_sensors,cpu_temp_only,has_battery;
    extern string cpuName,cpuHz;
    extern vector<string> available_fields;
    extern vector<string> available_sensors;
    extern tuple<int,long,string> current_bat;
    extern string gpu_name;
    extern string gpu_clock;
    extern bool has_gpu;
    extern atomic<uint64_t> smiTimer;
    extern atomic<uint64_t> OHMRTimer;

    struct GpuRaw{
        uint64_t usage=0;
        uint64_t mem_total=0;
        uint64_t mem_used=0;
        uint64_t temp=0;
        bool cpu_gpu=false;
        string clock_mhz;
    };

    struct OHMRraw{
        unordered_flat_map<string,GpuRaw> GPUS;
        vector<int> CPU;
        int CpuClock=0;
    };

    struct cpu_info{
        unordered_flat_map<string,deque<long long>> cpu_precent={
            {"taotal",{}},
            {"kernel",{}},
            {"user",{}},
            {"dpc",{}},
            {"interrupt",{}},
            {"idle",{}},
            {"gpu",{}}
        };
        vector<deque<long long>> core_precent;
        vector<deque<long long>> temp;
        deque<long long> gpu_temp;
        long long temp_max=100;
        array<float,3> load_avg;
    };

    auto collect(const bool no_update=false)->cpu_info&;

    string draw(const cpu_info& cpu,const bool force_redraw=false,const bool data_same=false);

    extern unordered_flat_map<int,int> core_mapping;

}

#endif

