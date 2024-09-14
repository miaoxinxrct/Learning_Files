#include "btop_term.hpp"

#include <windows.h>
#include <iostream>

#include "btop_mem.hpp"
#include "btop_cpu.hpp"
#include "btop_proc.hpp"
#include "btop_net.hpp"
#include "btop_global.hpp"

using std::cin,std::cout,std::flush;


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
            std::cout<<GetConsoleMode(handleOut,&out_saved_mode)<<" "<<GetConsoleMode(handleIn,&in_saved_mode);
            initialized=(GetConsoleMode(handleOut,&out_saved_mode) && GetConsoleMode(handleIn,&in_saved_mode));

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
