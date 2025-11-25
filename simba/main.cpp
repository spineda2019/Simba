// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>

#include "include/MainWindow.hpp"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    simba::MainWindow window{simba::MainWindow::New<600, 600>()};
    window.show();
    return Fl::run();
}
