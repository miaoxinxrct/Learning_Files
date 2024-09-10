#ifndef BTOP_LOGGER_HPP
#define BTOP_LOGGER_HPP

#include <vector>
#include <string>
#include <filesystem>

using std::vector,std::string;

namespace Logger{
    const vector<string> log_levels={
        "DISABLED",
        "ERROR",
        "WARNING",
        "INFO",
        "DEBUG",
    };
    extern std::filesystem::path logfile;


    //387
    void set(const string& level);
    //390
    void log_write(const size_t level,const string& msg);

    //394
    inline void error(const string msg){
        log_write(1,msg);
    }
    inline void warning(const string msg){
        log_write(2,msg);
    }
    inline void info(const string msg){
        log_write(3,msg);
    }
    inline void debug(const string msg){
        log_write(4,msg);
    }
}

#endif