// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#ifndef SIMBA_VIEWS_MAINWINDOW_HPP_
#define SIMBA_VIEWS_MAINWINDOW_HPP_

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

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
          grand_total_box{FrameXPosition<Width>::value,
                          FrameYPosition<Height>::value,
                          FrameWidth<Width>::value, FrameHeight<Height>::value,
                          "Balance: TODO"},
          load_alternate_button_{
              ButtonXPosition<Width>::value, ButtonYPosition<Height>::value,
              ButtonWidth<Width>::value, ButtonHeight<Height>::value,
              "Load Other Config"} {
        //
        grand_total_box.box(FL_UP_BOX);
        grand_total_box.labelfont(FL_BOLD + FL_ITALIC);
        grand_total_box.labelsize(36);

        const auto button_callback = [](Fl_Widget*, void* self) {
            static_cast<decltype(this)>(self)->LoadConfigClicked();
        };
        load_alternate_button_.type(FL_NORMAL_BUTTON);
        load_alternate_button_.callback(button_callback, this);

        window_.end();
    }

    template <int ParentWidth>
    struct FrameWidth {
        inline static constexpr int value{ParentWidth / 2};
    };

    template <int ParentHeight>
    struct FrameHeight {
        inline static constexpr int value{ParentHeight / 5};
    };

    template <int ParentWidth>
    struct FrameXPosition {
        inline static constexpr int value{
            (ParentWidth - FrameWidth<ParentWidth>::value) / 2};
    };

    template <int ParentWidth>
    struct FrameYPosition {
        // TODO(SEP): Calculate
        inline static constexpr int value{40};
    };

    template <int ParentWidth>
    struct ButtonWidth {
        inline static constexpr int value{ParentWidth / 4};
    };

    template <int ParentHeight>
    struct ButtonHeight {
        inline static constexpr int value{ParentHeight / 10};
    };

    template <int ParentWidth>
    struct ButtonXPosition {
        inline static constexpr int value{
            ((ParentWidth / 2) - ButtonWidth<ParentWidth>::value) / 2};
    };

    template <int ParentHeight>
    struct ButtonYPosition {
        inline static constexpr int value{
            FrameYPosition<ParentHeight>::value +
            FrameHeight<ParentHeight>::value +
            ((ParentHeight - (FrameYPosition<ParentHeight>::value +
                              FrameHeight<ParentHeight>::value)) /
             10)};
    };

 private:
    Fl_Window window_;
    Fl_Box grand_total_box;
    Fl_Button load_alternate_button_;
};
}  // namespace simba

#endif  // SIMBA_VIEWS_MAINWINDOW_HPP_
