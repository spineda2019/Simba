use fltk::prelude::FltkError;

mod main_window;

fn main() -> Result<(), FltkError> {
    let mut window = main_window::MainWindow::new(340, 340, "Hello FLTK-rs");
    window.show();

    fltk::app::run()
}
