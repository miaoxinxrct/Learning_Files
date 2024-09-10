#ifndef BTOP_DRAW_HPP
#define BTOP_DRAW_HPP

#include <string>
#include <vector>
#include <array>
#include <deque>
#include "robin_hood.h"

using std::string,std::array,std::vector,robin_hood::unordered_flat_map,std::deque;



namespace Draw{

    string banner_gen(int y=0,int x=0,bool centered=false,bool redraw=false);
    
    class TextEdit
    {
        public:
            string text;
            TextEdit();
            TextEdit(string text,bool numeric=false);
            bool command(const string& key);
            string operator()(const size_t limit=0);
            void clear();
        private:
            size_t _pos=0;
            size_t _upos=0;
            bool _numeric;
    };

    string createBox(const int x,const int y,const int width,const int height,string line_color="",
                    const bool fill=false,const string title="",const string title2="",const int num=0);
    
    bool update_clock(bool force=false);

    class Meter{
        public:
            Meter();
            Meter(const int width,const string& color_gradient,const bool invert=false);
            string operator()(int value);
        private:
            int _width;
            string _color_gradient;
            bool _invert;
            array<string,101> cache;
    };

    class Graph{
        public:
            Graph();
            Graph(int width,int height,const string& color_gradient,const deque<long long>& data,const string& symbol="default",
                  bool invert=false,bool no_zero=false,long long max_value=0,long long offset=0);
            string& operator()(const deque<long long>& data,const bool data_same=false);
            string& operator()();
        private:
            void _create(const deque<long,long>& data,int data_offset);

            int _width,_height;
            string _color_gradient;
            string _out,symbol="default";
            bool _invert,_no_zero;
            long long _offset;
            long long _last=0,_max_value=0;
            bool _currrent=true,_tty_mode=false;
            unordered_flat_map<bool,vector<string>> graphs={{true,{}},
                                                            {false,{}}};
            
    };

    void calcSizes();
}


#endif