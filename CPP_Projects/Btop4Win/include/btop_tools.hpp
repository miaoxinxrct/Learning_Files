#ifndef BTOP_TOOLS_HPP
#define BTOP_TOOLS_HPP

#include <atomic>
#include <string>
#include <chrono>
#include <filesystem>
#include <vector>
#include <regex>
#include <array>
#include <thread>


using std::atomic,std::string,std::vector,std::array;


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

    inline void sleep_ms(const size_t& ms){
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    string sec_to_dhms(size_t seconds,bool no_days=false,bool no_seconds=false);
    //344
    string strf_time(const string& strf);

    void atomic_wait(const atomic<bool>& atom,const bool old) noexcept;

    void atomic_wait_for(const atomic<bool>& atom,const bool old,const uint64_t waits_ms) noexcept;

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



#endif