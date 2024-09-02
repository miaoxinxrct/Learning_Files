#ifndef BTOP_SHARED_HPP
#define BTOP_SHARED_HPP

#include <string>
#include <atomic>
#include <tuple>
#include <robin_hood.h>
#include <deque>
#include <array>
#include <vector>

using std::string,std::atomic,std::vector,std::tuple,std::deque,std::array;
using robin_hood::unordered_flat_map;


namespace Global{
    extern const string Version;
    extern atomic<bool> resized;
}

namespace Shared{
    void init();
    extern long coreCount,page_size,clk_tck;
}

//79
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

namespace Mem{
    extern string box;
    extern int x,y,width,height,min_width,min_height;
    extern bool has_swap,shown,redraw;
    extern int disk_ios;
    extern int64_t totalMem;
    extern bool cpu_gpu;

    struct disk_info{
        string name;
        int64_t total=0,used=0,free=0;
        int used_precent=0,free_percent=0;
        array<int64_t,3> old_io={0,0,0};
        deque<long long> io_read={};
        deque<long long> io_write={};
        deque<long long> io_activity={};
    };

    struct mem_info{
        unordered_flat_map<string,uint64_t> stats={
            {"taotal",0},
            {"used",0},
            {"available",0},
            {"commit",0},
            {"commit_total",0},
            {"cached",0},
            {"page_total",0},
            {"page_used",0},
            {"page_free",0},
            {"gpu_total",0},
            {"gpu_used",0},
            {"gpu_free",0}
        };
        unordered_flat_map<string,deque<long long>> precent={
            {"used",{}},
            {"available",{}},
            {"commit",{}},
            {"cached",{}},
            {"page_used",{}},
            {"page_free",{}},
            {"gpu_used",{}},
            {"gpu_free",{}}
        };
        unordered_flat_map<string,disk_info> disks;
        vector<string> disk_order;
        bool pagevirt=false;
    };
    auto collect(const bool no_update=false)->mem_info&;
    string draw(const mem_info& mem,const bool force_redraw=false,const bool data_same=false);
}

namespace Net{
    extern string box;
    extern int x,y,width,height,min_width,min_height;
    extern bool shown,redraw;
    extern string selected_iface;
    extern vector<string> interfaces;
    extern bool rescale;
    extern unordered_flat_map<string,uint64_t> graph_map;

    struct net_stat{
        uint64_t speed=0,top=0,total=0,last=0,offset=0,rollover=0;
    };

    struct net_info{
        unordered_flat_map<string,deque<long long>> bandwidth={
            {"download",{}},
            {"upload",{}}
        };
        unordered_flat_map<string,net_stat> stat={
            {"download",{}},
            {"upload",{}}
        };
        string ipv4="",ipv6="";
        bool connected=false;
    };
    extern unordered_flat_map<string,net_info> current_net;
    auto collect(const bool no_update=false)->net_info&;
    string draw(const net_info& net,const bool force_redraw=false,const bool data_same=false);
}

namespace Proc{
    extern atomic<int> numpids;
    extern string box;
    extern int x,y,width,height,min_width,min_height;
    extern bool shown,redraw;
    extern int select_max;
    extern atomic<int> detailed_pid;
    extern int selected_pid,start,selected,collapse,expand,selected_depth;
    extern string selected_name;
    extern string selected_status;
    extern atomic<uint64_t> WMItimer;
    extern bool services_swap;
    
    //214
    const vector<string> sort_vector={
        "pid",
        "name",
        "command",
        "threads",
        "user",
        "memory",
        "cpu direct",
        "cpu lazy"
    };
    //226
    const vector<string> sort_vector_service={
        "service",
        "caption",
        "status",
        "memory",
        "cpu direct",
        "cpu lazy",

    };

    //236
    const unordered_flat_map<char,string> proc_states={
        {'R',"Running"},
        {'S',"Sleep"},
        {'D',"Waitting"},
        {'Z',"Zombie"},
        {'T',"Stopped"},
        {'t',"Tracing"},
        {'X',"Dead"},
        {'x',"Dead"},
        {'K',"Wakekill"},
        {'W',"Unknown"},
        {'P',"Parked"}
    };

    //251
    struct proc_info{
        size_t pid=0;
        string name="";
        string cmd="";
        string short_cmd="";
        size_t threads=0;
        string user="";
        uint64_t mem=0;
        double cpu_p=0.0,cpu_c=0.0;
        char states='0';
        uint64_t ppid=0,cpu_s=0,cpu_t=0,p_nice=0;
        string prefix="";
        size_t depth=0,tree_index=0;
        bool collapsed=false,filtered=false,WMI=false;
    };
    struct detail_container{
        size_t last_pid=0;
        proc_info entry;
        string elapsed,parent,status,io_read,io_write,memory,owner,start,description,last_name,service_type;
        long long first_mem=-1;
        deque<long long> cpu_percent;
        deque<long long> mem_bytes;
        double mem_percent=0.0;
        bool can_pause=false;
        bool can_stop=false;
    };

    extern detail_container detailed;
    auto collect(const bool no_update=false)->vector<proc_info>&;
    int selection(const string& cmd_key);
    string draw(const vector<proc_info>& plist,const bool force_redraw=false,const bool data_same=false);
}
#endif