// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>

int main(int argc, char **argv) {
  Fl_Window window(340, 180);
  Fl_Box box(20, 40, 300, 100, "Hello World!");
  box.box(FL_UP_BOX);
  box.labelfont(FL_BOLD + FL_ITALIC);
  box.labelsize(36);
  box.labeltype(FL_SHADOW_LABEL);
  window.end();
  window.show(argc, argv);
  return Fl::run();
}
