#ifndef BTOP_GLOBAL_HPP
#define BTOP_GLOBAL_HPP

#include <string>
#include <atomic>

using std::string,std::atomic;

namespace Global{
    extern const string Version;
    

    extern bool arg_tty;
    extern bool arg_low_color;
    extern int arg_preset;
    extern bool debug;

    extern string exit_error_msg;

    extern atomic<bool> resized;
    extern atomic<bool> quitting;

    extern uint64_t start_time;

    const string bg_black="\033[0;40m";
    const string fg_white="\033[1;97m";
    const string fg_green="\033[1;92m";
    const string fg_red="\033[0;91m";
}

#endif