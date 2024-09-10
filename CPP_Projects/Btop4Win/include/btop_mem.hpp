#ifndef BTOP_MEM_HPP
#define BTOP_MEM_HPP

#include <string>
#include <array>
#include <deque>

#include "robin_hood.h"

using std::string,std::deque,std::array;

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



#endif

