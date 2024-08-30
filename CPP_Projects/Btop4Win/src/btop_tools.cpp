#include "btop_tools.hpp"

#include <string_view>
#include <exception>
#include <iostream>
#include <fstream>

#include <btop_shared.hpp>


using std::string_view;
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

namespace Logger{
    using namespace Tools;
    std::atomic<bool> busy(false);
    bool first=true;
    const string tdf="%Y/%m/%d (%T) |";

    size_t logLevel;
    fs::path logfile;
    
    //699
    void set(const string& level){
        logLevel=v_index(log_levels,level);
    }

    //703
    void log_write(const size_t level,const string& msg){
        if(logLevel<level or logfile.empty())
            return;
        atomic_lock lck(busy,true);
        std::error_code ec;
        try{
            std::uintmax_t temp=fs::file_size(logfile,ec);
            auto temp2=fs::file_size(logfile,ec)>1024 << 10;
            if(fs::exists(logfile) and fs::file_size(logfile,ec)>1024 << 10 and not ec){
                auto old_log=logfile;
                old_log+=".1";
                if(fs::exists(old_log))
                    fs::remove(old_log,ec);
                if(not ec)
                    fs::rename(logfile,old_log,ec);
            }
            if(not ec){
                std::ofstream lwrite(logfile,std::ios::app);
                if(first){
                    first=false;
                    lwrite<<"\n"<<strf_time(tdf)<<"===> btop++ v."<<Global::Version<<"\n";
                }
                lwrite<<strf_time(tdf)<<log_levels.at(level)<<": "<<msg<<"\n";
            }
            else
                logfile.clear();
        }
        catch(const std::exception& e){
            logfile.clear();
            throw std::runtime_error("Exception in Logger::log_write() : "+(string)e.what());
        }
    }
}

