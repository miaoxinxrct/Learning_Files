#ifndef BTOP_SHARED_HPP
#define BTOP_SHARED_HPP

#include <string>
#include <atomic>

using std::string,std::atomic,std::vector;

namespace Global{
    extern const string Version;
}

namespace Proc{
    extern atomic<int> numpids;
    
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
}
#endif