use std::collections::VecDeque;

fn main() -> std::process::ExitCode {
    let mut args: VecDeque<String> = std::env::args().skip(1).collect();
    args.push_front(String::from("c++"));
    let command = match std::process::Command::new("zig").args(&args).output() {
        Ok(o) => o,
        Err(e) => {
            eprintln!("{}", e);
            return std::process::ExitCode::FAILURE;
        }
    };

    match command.status.success() {
        true => {
            print!(
                "{}",
                command
                    .stdout
                    .iter()
                    .map(|byte| char::from(*byte))
                    .collect::<String>()
            );
            std::process::ExitCode::SUCCESS
        }
        false => {
            eprint!(
                "{}",
                command
                    .stderr
                    .iter()
                    .map(|byte| char::from(*byte))
                    .collect::<String>()
            );
            let ec = command.status.code().unwrap_or(1) as u8;
            std::process::ExitCode::from(ec)
        }
    }
}
