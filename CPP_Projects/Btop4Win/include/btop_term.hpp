#ifndef BTOP_TERM_HPP
#define BTOP_TERM_HPP

#include <string>
#include <atomic>
#include <array>

#include "btop_fx.hpp"

using std::string,std::atomic,std::array;

namespace Term{
    extern atomic<bool> initialized;
    extern atomic<int> width;
    extern atomic<int> height;
    extern string fg,bg,current_tty;

    const string hide_cursor=Fx::e+"?251";
    const string show_cursor=Fx::e+"?25h";
    const string alt_screen=Fx::e+"?1049h";
    const string normal_screen=Fx::e+"?1049l";
    const string clear=Fx::e+"2J"+Fx::e+"0;0f";
    const string clear_end=Fx::e+"0J";
    const string clear_begin=Fx::e+"1J";
    const string sync_start=Fx::e+"?2026h";
    const string sync_end=Fx::e+"?2026l";

    bool refresh(bool only_check=false);
    auto get_min_size(const string& boxes)->array<int,2>;
    bool init();
    void restore();
}



#endif