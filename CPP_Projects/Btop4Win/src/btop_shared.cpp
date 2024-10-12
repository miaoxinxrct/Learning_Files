#include "btop_shared.hpp"
#include "btop_cpu.hpp"
#include "btop_config.hpp"

#include <filesystem>

namespace fs=std::filesystem;

namespace Shared{
    fs::path procPath,passwd_path;
    long pageSize,clkTck,coreCount;
    void init(){
        procPath="";
        passwd_path="";
       // Cpu::OHMR_path=Config::conf_dir.string()+"OHMR\\OpenHardwareMonitorReport.exe";
       // Cpu::OHMR_out_path=Config::conf_dir.string()+"OHMR\\out";
    }
}