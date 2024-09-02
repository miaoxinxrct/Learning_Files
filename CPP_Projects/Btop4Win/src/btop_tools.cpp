#include "btop_tools.hpp"

#include <string_view>
#include <exception>
#include <iostream>
#include <fstream>
#include <windows.h>

#include <btop_shared.hpp>


using std::string_view,std::cin,std::cout,std::flush;
namespace fs =std::filesystem;




namespace Term{
    atomic<bool> initialized=false;
    atomic<int> width=0;
    atomic<int> height=0;
    string current_tty;
    DWORD out_saved_mode;
    DWORD in_saved_mode;

    bool refresh(bool only_check){
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if(not GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi))
            return false;
        if(width!=csbi.srWindow.Right-csbi.srWindow.Left+1 or height!=csbi.srWindow.Bottom-csbi.srWindow.Top+1){
            if(not only_check){
                width=csbi.srWindow.Right-csbi.srWindow.Left+1;
                height=csbi.srWindow.Bottom-csbi.srWindow.Top+1;
            }
            return true;
        }
        return false;
    }

    auto get_min_size(const string& boxes)->array<int,2>{
        const bool cpu=boxes.find("cpu")!=string::npos;
        const bool mem=boxes.find("mem")!=string::npos;
        const bool net=boxes.find("net")!=string::npos;
        const bool proc=boxes.find("proc")!=string::npos;

        int width=0;
        if(mem)
            width=Mem::min_width;
        else if(net)
            width=Mem::min_width;
        width+=(proc?Proc::min_width:0);
        if(cpu and width<Cpu::min_width)
            width=Cpu::min_width;

        int height=(cpu?Cpu::min_height:0);
        if(proc)
            height+=Proc::min_height;
        else
            height+=(mem?Mem::min_height:0)+(net?Net::min_height:0);
        
        return {width,height};
    }

    void set_modes(){
        static HANDLE handleOut=GetStdHandle(STD_OUTPUT_HANDLE);
        static HANDLE handleIn=GetStdHandle(STD_INPUT_HANDLE);

        DWORD out_consoleMode=out_saved_mode;
        out_consoleMode!=(ENABLE_VIRTUAL_TERMINAL_PROCESSING|DISABLE_NEWLINE_AUTO_RETURN);
        SetConsoleMode(handleOut,out_consoleMode);
        SetConsoleOutputCP(65001);

        DWORD in_consoleMode=0;
        in_consoleMode=ENABLE_WINDOW_INPUT|ENABLE_MOUSE_INPUT|ENABLE_EXTENDED_FLAGS;
        in_consoleMode &= ~ENABLE_ECHO_INPUT;
        SetConsoleMode(handleIn,in_consoleMode);
    }

    bool init(){
        if(not initialized){
            HANDLE handleOut=GetStdHandle(STD_OUTPUT_HANDLE);
            HANDLE handleIn=GetStdHandle(STD_INPUT_HANDLE);
            initialized=(GetConsoleMode(handleOut,&out_saved_mode)&&GetConsoleMode(handleIn,&in_saved_mode));

            if(initialized){
                set_modes();
                cin.sync_with_stdio(false);
                cout.sync_with_stdio(false);

                cin.tie(NULL);
                cout.tie(NULL);
                refresh();

                cout<<alt_screen<<hide_cursor<<flush;
                Global::resized=false;
            }
        }
        return initialized;
    }

    void restore(){
        HANDLE handleOut=GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE handleIn=GetStdHandle(STD_INPUT_HANDLE);

        cout<<clear<<Fx::reset<<normal_screen<<show_cursor<<flush;

        SetConsoleMode(handleOut,out_saved_mode);
        SetConsoleMode(handleIn,in_saved_mode);

        initialized=false;
    }
}

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

