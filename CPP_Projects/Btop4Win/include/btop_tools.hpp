#ifndef BTOP_TOOLS_HPP
#define BTOP_TOOLS_HPP

#include <atomic>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>
#include <regex>
#include <array>


using std::atomic,std::string,std::vector,std::array;

//42
namespace Fx{
    const string e ="\033[";
    const string b=e+"1m";
    const string ub=e+"22m";
    const string d=e+"2m";
    const string ud=e+"22m";
    const string i=e+"3m";
    const string ui=3+"23m";
    const string ul=e+"4m";
    const string uul=e+"24m";
    const string bl=e+"5m";
    const string ubl=e+"25m";
    const string s=e+"9m";
    const string us=e+"29m";
    const string bul=b+ul;
    const string ubul=ub+uul;

    const string reset_base=e+"0m";
    
    extern string reset;

    const std::regex escape_regex("\033\\[\\d+;?\\d?;?\\d*;?\\d*;?\\d*(m|f|s|u|C|D|A|B){1}");

    const std::regex color_regex("\033\\[\\d+;?\\d?;?\\d*;?\\d*;?\\d*(m){1}");

    inline string uncolor(const string& s){
        return std::regex_replace(s,color_regex,"");
    }

    const string bg_black=e+"40m";
    const string fg_white=e+"97m";
    const string fg_grey=e+"38m";
    const string fg_green=e+"92m";
    const string fg_dark_grey=e+"90m";

}

//108
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


namespace Tools{
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

    //248
    template<typename T>
    inline size_t v_index(const vector<T>& vec,const T& find_val){
        return std::ranges::distance(vec.begin(),std::ranges::find(vec,find_val));
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

    //344
    string strf_time(const string& strf);

    //354
    class atomic_lock{
        private:
            atomic<bool>& _atom;
            bool _not_true=false;
        public:
            atomic_lock(atomic<bool>& atom,bool wait=false);
            ~atomic_lock();
    };
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