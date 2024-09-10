#ifndef BTOP_DRAW_HPP
#define BTOP_DRAW_HPP

#include <string>
#include <vector>
#include <array>
#include <deque>
#include "robin_hood.h"

namespace Proc{
    extern Draw::TextEdit filter;
    extern unordered_flat_map<size_t,Draw::Graph> p_graphs;
    extern unordered_flat_map<size_t,int> p_counters;

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

    //extern vector<string> sort_vector;
   // extern vector<string> sort_vector_service;
    
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

