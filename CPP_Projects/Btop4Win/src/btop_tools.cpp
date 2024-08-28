#include "btop_tools.hpp"

#include <string_view>

using std::string_view;
namespace fs =std::filesystem;

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
    
    //509
    void atomic_wait(const atomic<bool>& atom,const bool old) noexcept
    {
        while(atom.load(std::memory_order_relaxed)==old);
    }

}

namespace Logger{
    using namespace Tools;
    std::atomic<bool> busy(false);
    bool first=true;
    const string tdf="%Y/%m/%d (%T) |";

    size_t logLevel;
    fs::path logfile;
}

