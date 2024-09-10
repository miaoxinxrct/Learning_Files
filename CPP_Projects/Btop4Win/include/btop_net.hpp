#ifndef BTOP_NET_HPP
#define BTOP_NET_HPP

#include <string>
#include <vector>

#include "robin_hood.h"

using std::string,std::vector;

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

#endif