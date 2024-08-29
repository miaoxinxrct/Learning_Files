#ifndef BTOP_TOOLS_HPP
#define BTOP_TOOLS_HPP

#include <atomic>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>


using std::atomic,std::string,std::vector;


namespace Tools
{
    constexpr auto SSmax=std::numeric_limits<std::streamsize>::max();

    void atomic_wait(const atomic<bool>& atom,const bool old) noexcept;

    //224
    template<typename T,typename T2>
    inline bool v_contains(const vector<T>& vec,const T2& find_value)
    {
        return std::ranges::find(vec,find_value) != vec.end();
    }
    //230
    template<typename T>
    inline bool s_contains(const string& str,const T& find_val){
        return str.find(find_val) != string::npos;
    }

    //254
    template<typename First,typename ...T>
    inline bool is_in(const First& first,const T& ...t)
    {
        return ((first==t) or ...);
    }

    //258
    inline uint64_t time_s(){
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    inline uint64_t time_ms(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }


    inline uint64_t time_micros(){
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    //273
    inline bool isbool(const string& str)
    {
        return is_in(str,"true","false","True","False");
    }
    //278
    inline bool stobool(const string& str){
        return is_in(str,"true","True");
    }

    //284
    inline bool isint(const string& str){
        return all_of(str.begin(),str.end(),::isdigit);
    }

    //288
    string ltrim(const string& str,const string& t_str=" ");

    //291
    string rtrim(const string& str,const string& t_str=" ");
    
    //294
    inline string trim(const string& str,const string& t_str=" "){
        return ltrim(rtrim(str,t_str),t_str);
    }

    //310
    auto ssplit(const string& str,const char& delim=' ')->vector<string>;
}

namespace Logger{
    const vector<string> log_levels={
        "DISABLED",
        "ERROR",
        "WARNING",
        "INFO",
        "DEBUG",
    };
    extern std::filesystem::path logfile;
}

#endif