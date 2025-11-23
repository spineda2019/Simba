use fltk::prelude::{GroupExt, WidgetBase, WidgetExt};

pub struct MainWindow {
    inner_frame: fltk::frame::Frame,
    window: fltk::window::Window,
}

impl MainWindow {
    pub fn new(width: crate::common::Width, height: crate::common::Height, title: &str) -> Self {
        let window = fltk::window::Window::new(0, 0, width.0, height.0, title);
        let mut frame = fltk::frame::Frame::new(20, 40, width.0 / 2, height.0 / 2, title);
        frame.set_frame(fltk::enums::FrameType::UpBox);
        frame.set_label_size(36);
        window.end();

        MainWindow {
            inner_frame: frame,
            window,
        }
    }

    pub fn show(&mut self) {
        _ = self.inner_frame;
        self.window.show()
    }
}
