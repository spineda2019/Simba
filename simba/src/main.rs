use fltk::prelude::FltkError;

mod common;
mod views;

fn main() -> Result<(), FltkError> {
    let mut window = views::main_window::MainWindow::new(
        common::Width(600),
        common::Height(600),
        "Hello FLTK-rs",
    );
    window.show();

    fltk::app::run()
}
