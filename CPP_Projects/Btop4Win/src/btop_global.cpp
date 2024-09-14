#include "btop_global.hpp"

#include <array>
#include <vector>
#include <filesystem>

using std::array,std::vector;
namespace fs=std::filesystem;


namespace Global
{
    const vector<array<string,2>> Banner_src={
        {"#E62525", "¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨["},
		{"#CD2121", "¨€¨€¨X¨T¨T¨€¨€¨[¨^¨T¨T¨€¨€¨X¨T¨T¨a¨€¨€¨X¨T¨T¨T¨€¨€¨[¨€¨€¨X¨T¨T¨€¨€¨[   ¨€¨€¨[    ¨€¨€¨["},
		{"#B31D1D", "¨€¨€¨€¨€¨€¨€¨X¨a   ¨€¨€¨U   ¨€¨€¨U   ¨€¨€¨U¨€¨€¨€¨€¨€¨€¨X¨a ¨€¨€¨€¨€¨€¨€¨[¨€¨€¨€¨€¨€¨€¨["},
		{"#9A1919", "¨€¨€¨X¨T¨T¨€¨€¨[   ¨€¨€¨U   ¨€¨€¨U   ¨€¨€¨U¨€¨€¨X¨T¨T¨T¨a  ¨^¨T¨€¨€¨X¨T¨a¨^¨T¨€¨€¨X¨T¨a"},
		{"#801414", "¨€¨€¨€¨€¨€¨€¨X¨a   ¨€¨€¨U   ¨^¨€¨€¨€¨€¨€¨€¨X¨a¨€¨€¨U        ¨^¨T¨a    ¨^¨T¨a"},
		{"#000000", "¨^¨T¨T¨T¨T¨T¨a    ¨^¨T¨a    ¨^¨T¨T¨T¨T¨T¨a ¨^¨T¨a"},
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

