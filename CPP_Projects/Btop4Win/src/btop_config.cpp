#include <array>
#include <ranges>
#include <atomic>
#include <fstream>
#include <string_view>

#include "btop_config.hpp"
#include "btop_tools.hpp"

using std::array,std::atomic,std::string_view,std::string_literals::operator""s;
namespace fs=std::filesystem;
namespace rng=std::ranges;
using namespace Tools;

namespace Config{
    atomic<bool> locked(false);
    atomic<bool> writelock(false);
    bool write_new;

    const vector<array<string,2>> descriptions={
        {"color_theme",         "#* Name if a btop++/bpytop/bashtop formatted \".theme\" file,\"Default\" and \"TTY\" for builtin themes.\n"
                                "#* Themes should be placed in \"themes\" folder in same folder as btop4win.exe"},
        {"theme_backgroup",     "#* If the theme set backgroud should be shown, set to False if you want terminal background transparency."},
        {"truecolor",           "#* Sets if 24-bit truecolor should be used, will convert 24-bit colors to 256 color (6x6x6 color cube) if false"},
        {"force_tty",           "#* Set to true to force tty mode regardless if a real tty has been detected or not.\n"
                                "#* Will force 16-color mode and TTY theme, set all graph symbols to \"tty\" and swap out other non tty friendly symbols."},
        {"presets",             "#* Define preset for the layout of boxes. Preset 0 is always all boxes shown with default settings.Max 9 preset.\n"
                                "#* Format: \"box_name:P:G,box_name:P:G\" P=(0 or 1) for alternate positions, G=graph symbol to use for box.\n"
                                "#* Use whitespace \" \" as separator between different presets.\n"
                                "#* Example: \"cpu:0:default,mem:0:tty,proc:1:default cpu:0:braille,proc:0:tty\""},
        {"vim_keys",            "#* Set to true to enable \"h,j,k,l,g,G\" keys for directional control in lists.\n"
                                "#* Confilicting keys for h:\"help\" and k:\"kill\" is accessible while holding shift."},
        {"enable_ohmr",         "#* Enables monitoring of CPU temps, accurate CPU clock and GPU via Libre hardware Monitor.\n"
                                "#* Needs the my DLL's from(https://github.com/aristocratos/LHM-CPPExport) installed in same folder as btop4win.exe"},
        {"show_gpu",            "#* Also show gpu stats in cpu and mem box. Needs libre hardware monitor report enabled."},
        {"selected_gpu",        "#* Which GPU to display if multiple is detected."},
        {"gpu_mem_override",    "#* Manually set and override the GPU total memory shown when not correctly detected. value in Mib. Example: \"gpu_mem_override=1024\" for 1Gib"},
        {"rounded_corners",     "#* Rounded corners on boxes, is ignored if TTY mode is ON."},
        {"graph_symbol",        "#* Default symbols to use for graph creation,\"braille\",\"block\" or \"tty\".\n"
                                "#* \"braille\" offers the highest resolution but might not be included in all fonts.\n"
                                "#* \"block\" has half the resolution of braille but uses more common characters.\n"
                                "#* \"tty\" uses only 3 different symbols but will work with most fonts and should work in a really TTY.\n"
                                "#* Note that \"tty\" only has half the horizontal resolution of the other two, so will show a shorter historical view."},
        {"graph_symbol_cpu",    "# Graph symbol to use for graphs in cpu box, \"default\", \"braille\", \"block\" or \"tty\"."},
       	{"graph_symbol_mem", 	"# Graph symbol to use for graphs in cpu box, \"default\", \"braille\", \"block\" or \"tty\"."},
		{"graph_symbol_net", 	"# Graph symbol to use for graphs in cpu box, \"default\", \"braille\", \"block\" or \"tty\"."},
		{"graph_symbol_proc", 	"# Graph symbol to use for graphs in cpu box, \"default\", \"braille\", \"block\" or \"tty\"."},
        {"shown_boxes",         "#* Manually set which boxes to show. Available values are \"cpu mem net proc\", separate values with whitespace."},
        {"update_ms",           "#* Update time in milliseconds, recommended 2000ms or above for better sample times for graphs."},
        {"proc_sorting",        "#* Processes sorting, \"pid\" \"program\" \"argument\" \"threads\" \"user\" \"memory\" \"cpu lazy\" \"cpu direct\",\n"
                                "#* \"cpu lazy\" sorts top process over time (easier to follow), \"cpu direct\" updates top process directly."},
        {"proc_services",       "#* Show services in the process box instead of processes."},
        {"services_sorting",    "#* Services sorting, \"service\" \"caption\" \"status\" \"memory\" \"cpu lazy\" \"cpu direct\",\n"
                                "#* \"cpu lazy\" sorts top process over time (easier to follow), \"cpu direct\" updates top process directly."},
        {"proc_reversed",       "#* Reverse sorting order, True or False."},
        {"proc_tree",           "#* Show processes as a tree."},
        {"proc_colors",         "#* Use the cpu graph colors in the process list."},
        {"proc_gradient",       "#* Use a darkening gradient in the process list."},
        {"proc_per_core",       "#* If process cpu usage should be of the core it's running on or usage of the total available cpu power."},
        {"proc_mem_bytes",      "#* Show process memory as bytes instead of percent."},
        {"proc_left",           "#* Show proc box on left side of screen instead of right."},
        {"cpu_graph_upper",     "#* Sets the CPU stat shown in upper half of the CPU graph, \"total\" is always availiable.\n"
                                "#* Select from a list  of detected attributes from options menu."},
        {"cpu_graph_lower",     "#* Sets the CPU stat shown in lower half of the CPU graph, \"total\" is always availiable.\n"
                                "#* Select from a list  of detected attributes from options menu."},
        {"cpu_invert_lower",    "#* Toggles if the lower CPU graph should be inverted."},
        {"cpu_single_graph",    "#* Set to True to completely disable the lower CPU graph."},
        {"cpu_buttom",          "#* Show CPU box at bottom of screen instead of top."},
        {"show_uptime",         "#* Show the system uptime in the CPU box."},
        {"check_temp",          "#* Show CPU temperature."},
        {"cpu_sensor",          "#* Which sensor to use for temperature, use options menu to select from list of available sensors."},
        {"show_coretemp",       "#* Show temperatures for cpu cores also if check_temp is True and sensors has been found."},
        {"temp_scale",          "#* Which temperatures scale to use, available values: \"celsius\", \"fahrenheit\", \"kelvin\" and \"rankine\"."},
        {"base_10_sizes",       "#* Use base 10 for bits/bytes sizes, KB=1000 instead of KiB=1024."},
        {"clock_format",        "#* Draw a clock at top of screen, formatting according to strftime, empty string to disable.\n"
                                "#* Special formatting: /host=hostname | /user=username | /uptime=syetem uptime"},
        {"background_update",   "#* Update main UI in background when menus are showing, set this to false if the menus is flickering too much for comfort."},
        {"custom_cpu_name",     "#* Custom cpu model name, empty string to disable."},
        {"disks_filter",        "#* Optional filter for shown disks, should be full path of a mountpoint, separate multiple values with whitespace \" \" .n"
                                "#* Begin line with \"exclude=\" to change to exclude filter, otherwise defaults to \"most include\" filter. Example: disk_filter=\"exclude=D:\\ E:\\\" ."},
        {"mem_graphs",          "#* Show graphs instead of meters for memory values."},
        {"mem_below_net",       "#* Show mem box below net box instead of above."},
        {"show_page",           "#* If page memory should be shown in memory box."},
        {"show_disks",          "#* if mem box should be split to also show disk info."},
        {"only_physical",       "#* Filter out non physical disks. Set this to False to include network disks, RAM disks and similar."},
        {"disk_free_priv",      "#* Set to true to show available disk space for privileged users."},
        {"show_io_stat",        "#* Toggles if to activity % (disk busy time) should be shown in regular disk usage view."},
        {"io_mode",             "#* Toggles io mode for disks, showing big graphs for disk read/write speeds."},
        {"io_graph_combined",   "#* Set to True to show combined read/write io graphs in io mode."},
        {"io_graph_speeds",     "#* Set the top speed for the io graphs in Mib/s (100 by default), use format \"device:\\speed\" separate disks with whitespace \" \".\n"
                                "#* Example: \"C:\\100 D:\\20 G:\\1\"."},
        {"net_download",        "#* Set fixed values for network graphs in Mebibits. Is only used if net_auto is also set to False."},
        {"net_upload",          ""},
        {"net_auto",            "#* Use network graphs auto rescaling mode, ignores any values set above and rescales down to 10 Kibibytes at the lowest."},
        {"net_sync",            "#* Sync the auto scaling for download and upload to whichever currently has the highest scale."},
        {"net_iface",           "#* Starts with the Network Interface specified here."},
        {"show_battery",        "#* Show battery stats in top right if battery is present."},
        {"log_level",           "#* Set loglevel for \"~/.config/btop/btop.log\" levels are: \"ERROR\" \"WARNING\" \"INFO\" \"DEBUG\" .\n"
                                "#* The level set includes all lower levels, i.e. \"DEBUG\" will show all logging info."}                       
    };

    unordered_flat_map<string,string> strings={
        {"color_theme","Default"},
        {"show_boxes","cpu mem net proc"},
        {"graph_symbol","tty"},
        {"presets","cpu:1:default,proc:0:default cpu:0:default,mem:0:default,net:0:default cpu:0:block,net:0:tty"},
        {"graph_symbol_cpu","default"},
        {"graph_symbol_mem","default"},
        {"graph_symbol_net","default"},
        {"graph_symbol_proc","default"},
        {"proc_sorting","cpu lazy"},
        {"services_sorting","cpu lazy"},
        {"cpu_graph_upper","total"},
        {"cpu_graph_lower","gpu"},
        {"selected_gpu","Auto"},
        {"temp_scale","celsius"},
        {"clock_format","%X"},
        {"custom_cpu_name",""},
        {"disks_filter",""},
        {"io_graph_speeds",""},
        {"net_iface",""},
        {"log_level","WARNING"},
        {"proc_filter",""},
        {"proc_command",""},
        {"selected_name",""},
        {"selected_status",""},
        {"detailed_name",""}
    };
    unordered_flat_map<string,string> stringsTmp;

    unordered_flat_map<string,bool> bools={
        {"theme_background",true},
        {"truecolor",true},
        {"rounded_corners",false},
        {"proc_services",false},
        {"proc_reversed",false},
        {"proc_tree",false},
        {"proc_colors",true},
        {"proc_gradient",true},
        {"proc_per_core",false},
        {"proc_mem_bytes",true},
        {"proc_left",false},
        {"cpu_invert_lower",true},
        {"cpu_single_graph",false},
        {"cpu_bottom",false},
        {"show_uptime",true},
        {"check_temp",true},
        {"enable_ohmr",true},
        {"show_gpu",true},
        {"show_coretemp",true},
        {"background_update",true},
        {"mem_graphs",true},
        {"mem_below_net",false},
        {"show_page",true},
        {"show_disks",true},
        {"only_physical",true},
        {"show_io_stat",true},
        {"io_mode",true},
        {"base_10_sizes",false},
        {"io_graph_combined",false},
        {"net_auto",true},
        {"net_sync",false},
        {"show_battery",true},
        {"vim_keys",false},
        {"tty_mode",false},
        {"disk_free_priv",false},
        {"force_tty",false},
        {"lowcolor",false},
        {"show_detailed",false},
        {"proc_filtering",false}
    };
    unordered_flat_map<string,bool> boolsTmp;

    unordered_flat_map<string,int> ints={
        {"update_ms",1500},
        {"net_download",100},
        {"net_upload",100},
        {"detailed_pid",0},
        {"selected_pid",0},
        {"selected_depth",0},
        {"proc_start",0},
        {"proc_selected",0},
        {"proc_last_selected",0},
        {"gpu_mem_override",0}
    };
    unordered_flat_map<string,int> intsTmp;

    bool _locked(const string& name){
        atomic_wait(writelock,true);
        if(not write_new and rng::find_if(descriptions,[&name](const auto& a){return a.at(0)==name;})!=descriptions.end())
            write_new=true;
        return locked.load();
    }

    fs::path conf_dir;
    fs::path conf_file;

    string validError;
    //357
    bool intValid(const string& name,const string& value){
        int i_value;
        try{
            i_value=stoi(value);
        }
        catch(const std::invalid_argument&){
            validError="Invalid numerical value!";
            return false;
        }
        catch(const std::out_of_range&){
            validError="Value out of range!";
            return false;
        }
        catch(const std::exception& e){
            validError=(string)e.what();
            return false;
        }
        if(name == "update_ms" and i_value<100)
            validError="Config value update_ms set too low (<100).";
        else if(name =="update_ms" and i_value>86400000)
            validError="Config value update_ms set too high (>86400000).";
        else 
            return true;
        return false;
    }

    //387
    bool stringValid(const string& name,const string& value){
        if(name=="log_level" and not v_contains(Logger::log_levels,value))
            validError="Invalid log_level: "+value;
        else if(name=="graph_symbol" and not v_contains(valid_graph_symbols,value))
            validError="Invalid graph symbol identifier: "+value;
        else if(name.starts_with("graph_symbol_") and (value!="default" and not v_contains(valid_graph_symbols,value)))
            validError="Invalid graph symbol identifier for "+name+": "+value;
        // else if(name=="shown_boxes" and not value.empty() and not check_boxes(value))
        //     validError="Invalid box name(s) in shown_boxes";
        // else if(name=="presets" and not presetsValid(value))
        //     return false;
        // else if(name=="proc_sorting" and not v_contains(Proc::sort_vector,value))
        //     validError="Invalid process sorting option!"
        // else if(name=="services_sorting" and not v_contains(Proc::sort_vector_service,value))
        //     validError="Invalid serivices sorting option!"
        // else if(name=="io_graph_speeds"){
        //     const auto maps=ssplit(value);
        //     bool all_good=true;
        //     for(const auto& map:maps){
        //         const auto map_split=ssplit(map,'\\');
        //         if(map_split.size()!=2)
        //             all_good=false;
        //         else if(map_split.at(0).empty() or not isint(map_split.at(1)))
        //             all_good=false;
        //         if(not all_good){
        //             validError="Invalid formatting of io_graph_speeds!";
        //             return false;
        //         }
        //     }
        //     return true;
        // }
        else 
            return true;
        return false;
    }
    //488
    // bool check_boxes(const string& boxes){
    //     auto new_boxes=ssplit(boxes);
    //     for(auto& box:new_boxes){
    //         if(not v_contains(valid_boxes,box))
    //             return false;
    //     }
    //     current_boxes=move(new_boxes);
    //     return true;
    // }

    //521
    void load(const std::filesystem::path& conf_file,vector<string>& load_warnings)
    {
        if(conf_file.empty())
            return;
        else if(not fs::exists(conf_file)){
            write_new=true;
            return;
        }
        std::ifstream created(conf_file);
        if(created.good()){
            vector<string> valid_names;
            for(auto& n:descriptions)
                valid_names.push_back(n[0]);
            if(string v_string; created.peek()!='#' or (getline(created,v_string,'\n') and not s_contains(v_string,Global::Version)))
                write_new=true;
            while(not created.eof()){
                created>>std::ws;
                if(created.peek()=='#'){
                    created.ignore(SSmax,'\n');
                    continue;
                }
                string name,value;
                getline(created,name,'=');
                if(name.ends_with(' '))
                    name=trim(name);
                if(not v_contains(valid_names,name)){
                    created.ignore(SSmax,'\n');
                    continue;
                }
                created>>std::ws;

                if(bools.contains(name)){
                    created>>value;
                    if(not isbool(value))
                        load_warnings.push_back("Got an invalid bool value for config name: "+ name);
                    else
                        bools.at(name)=stobool(value);
                }
                else if(ints.contains(name)){
                    created>>value;
                    if(not isint(value))
                        load_warnings.push_back("Got an invalid integer value for config name: "+name);
                    else if(not intValid(name,value))
                        load_warnings.push_back(validError);
                    else
                        ints.at(name)=stoi(value);
                }
                else if(strings.contains(name)){
                    if(created.peek()=='"'){
                        created.ignore(1);
                        getline(created,value,'"');
                    }
                    else
                        created>>value;
                    
                    if(not stringValid(name,value))
                        load_warnings.push_back(validError);
                    else
                        strings.at(name)=value;
                }
                created.ignore(SSmax,'\n');
            }
            if(not load_warnings.empty())
                write_new=true;
        }
    }
}
