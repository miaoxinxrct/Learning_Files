#include <atomic>
#include <array>
#include <filesystem>
#include <string>
#include "robin_hood.h"

using std::string,std::vector,robin_hood::unordered_flat_map;

namespace Config
{
    extern std::filesystem::path conf_dir;
    extern std::filesystem::path conf_file;

    extern unordered_flat_map<string,string> strings;
    extern unordered_flat_map<string,string> stringsTmp;
    extern unordered_flat_map<string,bool> bools;
    extern unordered_flat_map<string,bool> boolsTmp;
    extern unordered_flat_map<string,int> ints;
    extern unordered_flat_map<string,int> intsTmp;

    


    //63
    bool _locked(const string& name);

    //81
    inline void set(const string& name,const bool& value)
    {
        if(_locked(name))
            boolsTmp.insert_or_assign(name,value);
        else
            bools.at(name)=value;
    }

    inline void set(const string& name,const int& value)
    {
        if(_locked(name))
            intsTmp.insert_or_assign(name,value);
        else
            ints.at(name)=value;
    }

    inline void set(const string& name,const string& value)
    {
        if(_locked(name))
            stringsTmp.insert_or_assign(name,value);
        else
            strings.at(name)=value;
    }
}