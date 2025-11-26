// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#ifndef SIMBA_VIEWS_MAINWINDOW_HPP_
#define SIMBA_VIEWS_MAINWINDOW_HPP_

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

#include "./common.hpp"

namespace simba {

class MainWindow final {
 public:
    template <int Width, int Height>
    struct MainWindowSize {};

    template <int Width, int Height>
    static inline MainWindow New() {
        return MainWindow{MainWindowSize<Width, Height>{}};
    }

    void Show() noexcept;

    void LoadConfigClicked();

    ~MainWindow() = default;
    explicit MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    explicit MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

 private:
    template <int Width, int Height>
    explicit inline MainWindow(MainWindowSize<Width, Height>) noexcept
        : window_{0, 0, Width, Height, "Simba"},
          grand_total_box{FrameInfo<Width, Height>::x,
                          FrameInfo<Width, Height>::y,
                          FrameInfo<Width, Height>::width,
                          FrameInfo<Width, Height>::height, "Balance: TODO"},
          load_alternate_button_{
              ButtonInfo<Width, Height>::x, ButtonInfo<Width, Height>::y,
              ButtonInfo<Width, Height>::width,
              ButtonInfo<Width, Height>::height, "Load Other Config"},
          breakdown_button_{
              ButtonInfo<Width, Height>::x, ButtonInfo<Width, Height>::y,
              ButtonInfo<Width, Height>::width,
              ButtonInfo<Width, Height>::height, "Load Other Config"} {
        //
        grand_total_box.box(FL_UP_BOX);
        grand_total_box.labelfont(FL_BOLD + FL_ITALIC);
        grand_total_box.labelsize(36);

        constexpr auto button_callback = [](Fl_Widget*, void* self) {
            static_cast<decltype(this)>(self)->LoadConfigClicked();
        };
        load_alternate_button_.type(FL_NORMAL_BUTTON);
        load_alternate_button_.callback(button_callback, this);

        window_.end();
    }

    template <int ParentWidth, int ParentHeight>
    struct FrameInfo {
        inline static constexpr int width{ParentHeight / 2};
        inline static constexpr int height{ParentHeight / 5};
        inline static constexpr int x{(ParentWidth - width) / 2};
        inline static constexpr int y{40};  // TOD(SEP): calculate
    };

    template <int ParentWidth, int ParentHeight>
    struct ButtonInfo {
        inline static constexpr int width{ParentWidth / 4};
        inline static constexpr int height{ParentHeight / 10};
        inline static constexpr int x{
            ((ParentWidth / 2) - ButtonInfo<ParentWidth, ParentHeight>::width) /
            2};
        inline static constexpr int y{
            FrameInfo<ParentWidth, ParentHeight>::y +
            FrameInfo<ParentWidth, ParentHeight>::height +
            ((ParentHeight - (FrameInfo<ParentWidth, ParentHeight>::y +
                              FrameInfo<ParentWidth, ParentHeight>::height)) /
             10)};
    };

 private:
    Fl_Window window_;
    Fl_Box grand_total_box;
    Fl_Button load_alternate_button_;
    Fl_Button breakdown_button_;
};
}  // namespace simba

#endif  // SIMBA_VIEWS_MAINWINDOW_HPP_
