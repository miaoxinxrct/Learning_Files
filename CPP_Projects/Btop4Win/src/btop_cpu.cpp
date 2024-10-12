#include "btop_cpu.hpp"
#include "btop_shared.hpp"
#include "btop_logger.hpp"

#include <wtypes.h>

#include <powerbase.h>
#pragma comment(lib,"PowrProf.lib")


//This part for Draw
namespace Cpu{
    int width_p=100,height_p=32;
    int min_width=60,min_height=8;
    int x=1,y=1,width=20,height;
    int b_columns,b_width,b_height;
    int graph_up_height;
    bool shown=true,redraw=true,mid_line=false;
    string box;
}

//This part for Shared
namespace Cpu{
    string cpuName;
    string cpuHz;
    string gpu_clock;
    bool has_battery=true;
    tuple<int,long,string> current_bat;
    string current_gpu="";
    const array<string,6> time_names={"kernel","user","dpc","interrupt","idle"};
    unordered_flat_map<string,long long> cpu_old={
        {"total",0},
        {"kernel",0},
        {"user",0},
        {"dpc",0},
        {"interrupt",0},
        {"idle",0},
        {"totals",0},
        {"idles",0}
    };

    typedef struct _PROCESSOR_POWER_INFORMATION{
        ULONG Number;
        ULONG MaxMhz;
        ULONG CurrentMhz;
        ULONG MhzLimit;
        ULONG MaxIdleState;
        ULONG CurrentIdleState;
    }PROCESSOR_POWER_INFORMATION,*PPROCESSOR_POWER_INFORMATION;

    string get_cpuHz(){
        static bool failed=false;
        if(failed)
            return "";
        uint64_t hz=0;
        string cpuhz;
        vector<PROCESSOR_POWER_INFORMATION> ppinfo(Shared::coreCount);
        if(CallNtPowerInformation(ProcessorInformation,nullptr,0,&ppinfo[0],Shared::coreCount*sizeof(PROCESSOR_POWER_INFORMATION))!=0){
            Logger::warning("Cpu::get_cpuHz() -> CallNtPowerInfomation() failed");
            failed=true;
            return "";
        }
    }
}
