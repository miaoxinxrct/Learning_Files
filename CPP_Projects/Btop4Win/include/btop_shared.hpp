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



namespace Shared{
    void init();
    extern long coreCount,page_size,clk_tck;
}

#endif