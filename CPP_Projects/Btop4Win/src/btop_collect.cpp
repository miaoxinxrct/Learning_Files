#include <fstream>
#include <ranges>
#include <cmath>
#include <numeric>
#include <mutex>
#include <chrono>
#include <locale>
#include <codecvt>
#include <semaphore>
#include <iostream>

// #define _WIN32_DCOM
// #define _WIN32_WINNT 0x0600
// #define NOMINMAX
// #define WIN32_LEAN_AND_MEAN
// #define VC_EXTRALEAN
#include <windows.h>

#include <winreg.h>
#pragma comment( lib, "Advapi32.lib" )
#include <winternl.h>
#pragma comment( lib, "ntdll.lib" )
#include <Pdh.h>
#pragma comment( lib, "Pdh.lib" )
//#include <atlstr.h>
#include <tlhelp32.h>
#include <Psapi.h>
#pragma comment( lib, "Psapi.lib")
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#include <winioctl.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include <powerbase.h>
#pragma comment(lib, "PowrProf.lib")

#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

#include <btop_shared.hpp>
#include <btop_config.hpp>
#include <btop_tools.hpp>
#include <btop_draw.hpp>

#ifdef LHM_Enabled
	#pragma comment(lib, "external\\CPPdll.lib")
	_declspec(dllexport) std::string FetchLHMValues();
	_declspec(dllexport) std::string FetchLHMReport();
#endif

using std::string_view;



//1028
// namespace Cpu{
//     string cpuName;
//     string cpuHz;
//     string gpu_clock;
//     bool has_battery=true;
//     tuple<int,long,string> current_bat;
//     string current_gpu="";
//     const array<string,6> time_names={"kernel","user","dpc","interrupt","idle"};
//     unordered_flat_map<string,long long> cpu_old={
//         {"total",0},
//         {"kernel",0},
//         {"user",0},
//         {"dpc",0},
//         {"interrupt",0},
//         {"idle",0},
//         {"totals",0},
//         {"idles",0}
//     };

//     typedef struct _PROCESSOR_POWER_INFORMATION{
//         ULONG Number;
//         ULONG MaxMhz;
//         ULONG CurrentMhz;
//         ULONG MhzLimit;
//         ULONG MaxIdleState;
//         ULONG CurrentIdleState;
//     }PROCESSOR_POWER_INFORMATION,*PPROCESSOR_POWER_INFORMATION;

//     string get_cpuHz(){
//         static bool failed=false;
//         if(failed)
//             return "";
//         uint64_t hz=0;
//         string cpuhz;
//         vector<PROCESSOR_POWER_INFORMATION> ppinfo(Shared::coreCount);
//         if(CallNtPowerInformation(ProcessorInformation,nullptr,0,&ppinfo[0],Shared::coreCount*sizeof(PROCESSOR_POWER_INFORMATION))!=0){
//             Logger::warning("Cpu::get_cpuHz()->CallNtPowerInformation() failed");
//             failed=true;
//             return "";
//         }
//         hz=ppinfo[0].CurrentMhz;
//         if(hz<=1 or hz>=1000000){
//             Logger::warning("Cpu::get_cpuHz() ->Got invalid cpu mhz value");
//             failed=true;
//             return "";
//         }
//         if(hz>=1000){
//             if(hz>=10000)
//                 cpuhz=to_string((int)round(hz/1000));
//             else
//                 cpuhz=to_string(round(hz/100)/10.0).sustr(0,3);
//             cpuhz+=" GHz";
//         }
//         else if(hz>0)
//             cpuhz=to_string((int)round(hz))+" MHz";
//         return cpuhz;
//     }
// }