#include "btop_tools.hpp"

#include <string_view>
#include <exception>
#include <iostream>
#include <fstream>
#include <windows.h>

#include <btop_shared.hpp>


using std::string_view,std::cin,std::cout,std::flush;
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
