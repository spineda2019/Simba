// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#ifndef SIMBA_INCLUDE_MAINWINDOW_HPP_
#define SIMBA_INCLUDE_MAINWINDOW_HPP_

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

namespace simba {

class MainWindow final {
 public:
    explicit MainWindow(int width, int height) noexcept;

    void show() noexcept;

 private:
    Fl_Window window_;
    Fl_Box grand_total_box;
    Fl_Button load_alternate_button_;
};
}  // namespace simba

#endif  // SIMBA_INCLUDE_MAINWINDOW_HPP_
