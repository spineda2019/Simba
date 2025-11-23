use fltk::prelude::FltkError;

mod views;

fn main() -> Result<(), FltkError> {
    let mut window = views::main_window::MainWindow::new(340, 340, "Hello FLTK-rs");
    window.show();

    fltk::app::run()
}
