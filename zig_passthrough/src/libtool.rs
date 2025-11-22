mod runner;

fn main() -> std::process::ExitCode {
    runner::run("lib", false)
}
