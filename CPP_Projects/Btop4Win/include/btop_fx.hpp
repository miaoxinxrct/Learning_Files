#ifndef BTOP_FX_HPP
#define BTOP_FX_HPP

#include <string>
#include <regex>

using std::string;

namespace Fx{
    const string e ="\033[";
    const string b=e+"1m";
    const string ub=e+"22m";
    const string d=e+"2m";
    const string ud=e+"22m";
    const string i=e+"3m";
    const string ui=3+"23m";
    const string ul=e+"4m";
    const string uul=e+"24m";
    const string bl=e+"5m";
    const string ubl=e+"25m";
    const string s=e+"9m";
    const string us=e+"29m";
    const string bul=b+ul;
    const string ubul=ub+uul;

    const string reset_base=e+"0m";
    
    extern string reset;

    const std::regex escape_regex("\033\\[\\d+;?\\d?;?\\d*;?\\d*;?\\d*(m|f|s|u|C|D|A|B){1}");

    const std::regex color_regex("\033\\[\\d+;?\\d?;?\\d*;?\\d*;?\\d*(m){1}");

    inline string uncolor(const string& s){
        return std::regex_replace(s,color_regex,"");
    }

    const string bg_black=e+"40m";
    const string fg_white=e+"97m";
    const string fg_grey=e+"38m";
    const string fg_green=e+"92m";
    const string fg_dark_grey=e+"90m";

}

#endif