// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#include "logic/filesystem.hpp"

#include <FL/Fl_Native_File_Chooser.H>

namespace simba::fs {
void GetConfigFile() {
    Fl_Native_File_Chooser chooser{};
    constexpr const char* filter{
        "Simba Config Files\t*.simba\nTxt Files\t*.txt"};
    chooser.filter(filter);
    chooser.show();
    chooser.filename();
}
}  // namespace simba::fs
