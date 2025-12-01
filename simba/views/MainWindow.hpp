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
          grand_total_box{FrameInfo<Width, Height>::X::value,
                          FrameInfo<Width, Height>::Y::value,
                          FrameInfo<Width, Height>::Width::value,
                          FrameInfo<Width, Height>::Height::value,
                          "Balance: TODO"},
          load_alternate_button_{ButtonInfo<Width, Height>::X::value,
                                 ButtonInfo<Width, Height>::Y::value,
                                 ButtonInfo<Width, Height>::Width::value,
                                 ButtonInfo<Width, Height>::Height::value,
                                 "Load Other Config"},
          breakdown_button_{ButtonInfo<Width, Height>::X::value,
                            ButtonInfo<Width, Height>::Y::value,
                            ButtonInfo<Width, Height>::Width::value,
                            ButtonInfo<Width, Height>::Height::value,
                            "Load Other Config"} {
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
        struct Width {
            inline static constexpr int value{ParentHeight / 2};
        };
        struct Height {
            inline static constexpr int value{ParentHeight / 5};
        };
        struct X {
            inline static constexpr int value{(ParentWidth - Width::value) / 2};
        };
        struct Y {
            inline static constexpr int value{40};  // TODO(SEP): calculate
        };
    };

    template <int ParentWidth, int ParentHeight>
    struct ButtonInfo {
        struct Width {
            inline static constexpr int value{ParentWidth / 4};
        };
        struct Height {
            inline static constexpr int value{ParentHeight / 10};
        };
        struct X {
            inline static constexpr int value{
                ((ParentWidth / 2) -
                 ButtonInfo<ParentWidth, ParentHeight>::Width::value) /
                2};
        };
        struct Y {
            inline static constexpr int value{
                FrameInfo<ParentWidth, ParentHeight>::Y::value +
                FrameInfo<ParentWidth, ParentHeight>::Height::value +
                ((ParentHeight -
                  (FrameInfo<ParentWidth, ParentHeight>::Y::value +
                   FrameInfo<ParentWidth, ParentHeight>::Height::value)) /
                 10)};
        };
    };

 private:
    Fl_Window window_;
    Fl_Box grand_total_box;
    Fl_Button load_alternate_button_;
    Fl_Button breakdown_button_;
};
}  // namespace simba

#endif  // SIMBA_VIEWS_MAINWINDOW_HPP_
