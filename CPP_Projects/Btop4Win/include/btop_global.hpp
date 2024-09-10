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
}

#endif