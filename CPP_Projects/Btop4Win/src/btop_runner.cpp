#include <atomic>
#include <semaphore>

#include "btop_tools.hpp"


using std::atomic;

using namespace Tools;

namespace Runner{
    atomic<bool> active(false);
    atomic<bool> stopping(false);

    std::binary_semaphore do_work(0);
    inline void thread_wait(){do_work.acquire();}
    inline void thread_trigger(){do_work.release();}

    void stop(){
        stopping=true;
        thread_trigger();
        atomic_wait_for(active,false,100);
        atomic_wait_for(active,true,100);
        stopping=false;
    }
}