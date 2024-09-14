#include "btop_theme.hpp"
#include "btop_tools.hpp"
#include "btop_fx.hpp"

namespace fs=std::filesystem;

string Fx::reset=reset_base;

namespace Theme{
    fs::path theme_dir;
    fs::path user_theme_dir;
}