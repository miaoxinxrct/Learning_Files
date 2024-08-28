#ifndef BTOP_THEME_HPP
#define BTOP_THEME_HPP

#include <filesystem>
#include "robin_hood.h"
#include <string>
#include <array>
#include <vector>

using std::string,robin_hood::unordered_flat_map,std::array,std::vector;

namespace Theme{
    extern std::filesystem::path theme_dir;
    extern std::filesystem::path user_theme_dir;

    extern vector<string> themes;

    // string hex_to_color(string hexa,const bool& t_to_256=false,const string& depth="fg");
    // string dec_to_color(int r,int g,int b,const bool& t_to_256=false,const string& depth="fg");
    // void updateThemes();
    // void setTheme();

    extern unordered_flat_map<string,string> colors;
    extern unordered_flat_map<string, array<int,3>> rgbs;
    extern unordered_flat_map<string,array<string,101>> gradients;

    inline const string& c(const string& name){ return colors.at(name); }
    inline const array<string,101>& g(string name){ return gradients.at(name);  }
    inline const array<int,3>& dec(string name){ return rgbs.at(name);}
}

#endif