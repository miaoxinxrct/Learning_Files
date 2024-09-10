#include "btop_logger.hpp"


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