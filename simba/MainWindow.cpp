// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#include "include/MainWindow.hpp"

#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>

namespace simba {
MainWindow::MainWindow(int width, int height) noexcept
    : window_{0, 0, width, height, "Simba"},
      grand_total_box{(width - (width / 2)) / 2, 40, width / 2, height / 5,
                      "Grand Total:"},
      load_alternate_button_{((width / 2) - (width / 5)) / 2, 0, 0, 0, "Load"} {
    grand_total_box.box(FL_UP_BOX);
    grand_total_box.labelfont(FL_BOLD + FL_ITALIC);
    grand_total_box.labelsize(36);

    load_alternate_button_.type(FL_NORMAL_BUTTON);

    window_.end();
}

void MainWindow::show() noexcept { window_.show(); }
}  // namespace simba
