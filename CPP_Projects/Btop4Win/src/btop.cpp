#include <thread>
#include <numeric>
#include <ranges>
//#include <math.h>
#include <iostream>
#include <semaphore>

#define _WIN32_DCOM
//#define _WIN32_WINNT 0x0600
//#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>


#include <vector>
#include <atomic>

#include "btop_draw.hpp"
#include "btop_tools.hpp"
#include "btop_config.hpp"
#include "btop_theme.hpp"

#include <filesystem>

using std::string,std::string_view,std::vector,std::atomic,std::endl,std::cout,std::min,std::flush;
using std::string_literals::operator""s,std::to_string;
using std::array;

namespace fs=std::filesystem;
namespace rng=std::ranges;

using namespace Tools;
using namespace std::chrono_literals;

namespace Global
{
    const vector<array<string,2>> Banner_src={
        {"#E62525", "�������������[ �����������������[ �������������[ �������������["},
		{"#CD2121", "�����X�T�T�����[�^�T�T�����X�T�T�a�����X�T�T�T�����[�����X�T�T�����[   �����[    �����["},
		{"#B31D1D", "�������������X�a   �����U   �����U   �����U�������������X�a �������������[�������������["},
		{"#9A1919", "�����X�T�T�����[   �����U   �����U   �����U�����X�T�T�T�a  �^�T�����X�T�a�^�T�����X�T�a"},
		{"#801414", "�������������X�a   �����U   �^�������������X�a�����U        �^�T�a    �^�T�a"},
		{"#000000", "�^�T�T�T�T�T�a    �^�T�a    �^�T�T�T�T�T�a �^�T�a"},
    };

    const string Version="1.0.4";

    int coreCount;
    string overlay;
    string clock;

    fs::path self_path;
    
    string exit_error_msg;
    atomic<bool> thread_exception(false);

    bool debuginit=false;
    bool debug=true;
    bool utf_force=false;
    uint64_t start_time;

    atomic<bool> resized(false);
    atomic<bool> quitting(false);
    atomic<bool> should_quit(false);
    atomic<bool> should_sleep(false);
    atomic<bool> _runner_started(false);

    bool arg_tty=false;
    bool arg_low_color=false;
    int arg_preset=-1;
}

void argumentParser(const int& argc,char** argv)
{
    for(int i=1;i<argc;++i)
    {
        const string argument=argv[i];
        if(is_in(argument,"-h","--help")){
            cout<<"usage: btop [-h] [-v] [-/+t] [-p <id>] [--utf-force] [--debug]\n\n"
                <<"optional arguments:\n"
                <<" -h, --help                  show this help message and exit\n"
                <<" -v, --version               show version info and exit\n"
                <<" -lc,--low-color             disable truecolor, converts 24-bit"
                <<" -t, --tty_on                force (ON) tty mode, max 16 colors and tty friendly graph symbols\n"
                <<" +t, --tty_off               force (OFF) tty mode\n"
                <<" -p, --preset <id>           statrt with preset, integer value between 0-9\n"
                <<" --debug                     start in DEBUG mode: shows microsecond timer for information collect\n"
                <<"                             and screen draw functions and sets loglevel to DEBUG\n"
                <<endl;
            exit(0);
        }
        else if(is_in(argument,"-v","--version")){
            cout<<"btop4win version: "<<Global::Version<<endl;
            exit(0);
        }
        else if(is_in(argument,"-lc","--low-color")){
            Global::arg_low_color=true;
        }
        else if(is_in(argument,"-t","--tty_on")){
            //Config::set("tty_mode",true);
            Global::arg_tty=true;
        }
        else if(is_in(argument,"+t","--tty_off")){
            //Config::set("tty_mode",false);
            Global::arg_tty=false;
        }
        else if(is_in(argument,"-p","--preset")){
            if(++i>=argc){
                cout<<"ERROR: Preset option needs an argument.\n";
                exit(1);
            }
            else if(const string val=argv[i];isint(val) and val.size()==1){
                Global::arg_preset=std::clamp(stoi(val),0,9);
            }
            else{
                cout<<"ERROR: Preset option only accepts an integer value between 0-9.\n";
                exit(1);
            }
        }
        else if(argument=="--debug")
            Global::debug=true;
        else{
            cout<<"Unknown argument: "<<argument<<"\n"
                <<" Use -h or --help for help.\n";
            exit(1);
        }
    }
}

void clean_quit(int sig){
    if(Global::quitting)
        return;
    Global::quitting=true;
    //Runner::stop();

    //Config::write();

    // if(Term::initialized){
    //     Term::restore();
    // }
    // if(not Global::exit_error_msg.empty()){
    //     sig=1;
    //     Logger::error(Global::exit_error_msg);
    //     std::cerr<<Global::fg_red<<"ERROR: "<<Global::fg_write<<Global::exit_error_msg<<Fx::reset<<endl;
    // }
    // Logger::info("Quitting! Runtime: "+sec_to_dhms(time_s()-Global::start_time));

    // const auto excode =(sig!=-1?sig:0);
    // quick_exit(excode);
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType){
    switch (fdwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case CTRL_CLOSE_EVENT:
        clean_quit(0);
        return TRUE;
    default:
        return FALSE;
    }
}

int main(int argc,char** argv){
    std::cout<<"Run Main\n";
    Global::start_time=time_s();

    if(argc>1)
        argumentParser(argc,argv);
    
    SetConsoleCtrlHandler(CtrlHandler,true);
    SetConsoleTitleA("btop4win++");

    wchar_t self_path[FILENAME_MAX]={0};
    GetModuleFileNameW(nullptr,self_path,FILENAME_MAX);


    Config::conf_dir=fs::path(self_path).remove_filename();
    Config::conf_file=Config::conf_dir/"btop.conf";
    Logger::logfile=Config::conf_dir/"btop.log";
    Theme::theme_dir=Config::conf_dir/"themes";
    std::cout<<Config::conf_dir.string()<<"\n"<<Config::conf_file.string()<<"\n"
             <<Logger::logfile.string()<<"\n"<<Theme::theme_dir.string();

    {
        vector<string> load_warnings;
        Config::load(Config::conf_file,load_warnings);

        if(Config::current_boxes.empty())
           Config::check_boxes(Config::getS("shown_boxes"));
        Config::set("lowcolor",(Global::arg_low_color?true:not Config::getB("truecolor")));

        if(Global::debug){
            Logger::set("DEBUG"),
            Logger::debug("Starting in DEBUG mode!");
        }
        
    }


    return 0;
}