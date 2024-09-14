#include "btop_tools.hpp"

#include <string_view>
#include <exception>
#include <iostream>
#include <fstream>
#include <windows.h>

#include <btop_shared.hpp>


using std::string_view,std::cin,std::cout,std::flush,std::to_string;
namespace fs =std::filesystem;

namespace Tools{

    //348
    string ltrim(const string& str,const string& t_str){
        string_view str_v=str;
        while(str_v.starts_with(t_str))
            str_v.remove_prefix(t_str.size());
        return (string)str_v;
    }

    //354
    string rtrim(const string& str,const string& t_str){
        string_view str_v=str;
        while(str_v.ends_with(t_str))
            str_v.remove_suffix(t_str.size());
        return (string)str_v;
    }

    auto ssplit(const string& str,const char& delim)->vector<string>{
        vector<string> out;
        if(str.empty())
            return out;
        size_t last=0;
        for(size_t loc=str.find(delim);loc!=std::string::npos;loc=str.find(delim,last)){
            out.push_back(str.substr(last,loc-last));
            last=loc+1;
        }
        if(str.size()-last>0)
            out.push_back(str.substr(last));
        return out;
    }

    string sec_to_dhms(size_t seconds,bool no_days,bool no_seconds){
        size_t days=seconds/86400;seconds%=86400;
        size_t hours=seconds/3600;seconds%=3600;
        size_t minutes=seconds/60;seconds%=60;
        string out = (not no_days and days >0?to_string(days)+"d ":"")
                   + (hours<10?"0":"")+to_string(hours)+":"
                   + (minutes<10?"0":"")+to_string(minutes)
                   + (not no_seconds?":"+string(std::cmp_less(seconds,10)?"0":"")+to_string(seconds):"");
        return out;
    }
    
    //501
    string strf_time(const string& strf){
        const time_t in_time_t=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        struct tm* bt=localtime(&in_time_t);
        ss<<std::put_time(bt,strf.c_str());
        return ss.str();
    }

    //509
    void atomic_wait(const atomic<bool>& atom,const bool old) noexcept
    {
        while(atom.load(std::memory_order_relaxed)==old);
    }

    void atomic_wait_for(const atomic<bool>& atom,const bool old,const uint64_t wait_ms) noexcept{
        const uint64_t start_time=time_ms();
        while(atom.load(std::memory_order_relaxed)==old and (time_ms() -start_time<wait_ms)) sleep_ms(1);   
    }

    atomic_lock::atomic_lock(atomic<bool>& atom,bool wait):_atom(atom){
        if(wait)
            while(not this->_atom.compare_exchange_strong(this->_not_true,true));
        else
            this->_atom.store(true);
    }

    atomic_lock::~atomic_lock(){
        this->_atom.store(false);
    }

}
