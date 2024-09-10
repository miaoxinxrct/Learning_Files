#include "btop_mem.hpp"


namespace Mem{
    int width_p=45,height_p=36;
    int min_width=36,min_height=10;
    int x=1,y,width=20,height;
    int mem_width,disks_width,divider,item_height,mem_size,mem_meter,graph_height,disk_meter;
    int disks_io_h=0;
    int disks_io_half=0;
    bool shown=true,redraw=true;
    string box;
}