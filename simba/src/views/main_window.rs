use std::path::{self, Path};

use fltk::{
    button::Button,
    dialog::{FileDialogType, NativeFileChooser},
    prelude::{GroupExt, WidgetBase, WidgetExt},
};

pub struct MainWindow {
    inner_frame: fltk::frame::Frame,
    window: fltk::window::Window,
}

impl MainWindow {
    const LEFT_MOST_X: i32 = 0;
    const TOP_MOST_Y: i32 = 0;

    pub fn new(width: crate::common::Width, height: crate::common::Height, title: &str) -> Self {
        let window = fltk::window::Window::new(
            Self::LEFT_MOST_X,
            Self::TOP_MOST_Y,
            width.0,
            height.0,
            title,
        );

        let frame_width = crate::common::Width(width.0 / 2);
        let frame_height = crate::common::Height(height.0 / 5);
        let frame_start_x = (width.0 - frame_width.0) / 2;
        let frame_start_y = 40;
        let mut frame = fltk::frame::Frame::new(
            frame_start_x,
            frame_start_y,
            frame_width.0,
            frame_height.0,
            "Grand Total:",
        );
        frame.set_frame(fltk::enums::FrameType::UpBox);
        frame.set_label_font(fltk::enums::Font::TimesBold);
        frame.set_label_size(36);

        let button_width = width.0 / 5;
        let button_start_y = frame_start_y + frame_height.0;
        let button_start_y = button_start_y + ((height.0 - button_start_y) / 10);
        let button_start_x = ((width.0 / 2) - button_width) / 2;
        let button_height = height.0 / 10;
        let mut load_button = Button::new(
            button_start_x,
            button_start_y,
            button_width,
            button_height,
            "Load Config",
        );
        load_button.set_type(fltk::button::ButtonType::Normal);
        load_button.set_callback(Self::load_button_click);

        window.end();
        MainWindow {
            inner_frame: frame,
            window,
        }
    }

    fn load_button_click(_: &mut Button) {
        let mut chooser = NativeFileChooser::new(FileDialogType::BrowseFile);
        chooser.set_filter("Simba Configuration Files\t*.{simba,sim}\nCSV Files\t*.csv");
        chooser.show();
        let path = chooser.filename();
        // if path.as_path(). {}
    }

    pub fn show(&mut self) {
        _ = self.inner_frame;
        self.window.show()
    }
}
