#include "btop_theme.hpp"

namespace fs=std::filesystem;

string Fx::reset=reset_base;

namespace Theme{
    fs::path theme_dir;
    fs::path user_theme_dir;
}