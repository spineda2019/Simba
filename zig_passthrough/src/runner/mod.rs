use std::collections::VecDeque;

pub fn run(subcommand: &str, needs_target_flag: bool) -> std::process::ExitCode {
    let mut args: VecDeque<String> = std::env::args().skip(1).collect();
    args.push_front(String::from(subcommand));
    if needs_target_flag {
        const ZIG_TRIPLE: &str = env!("ZIG_TRIPLE");
        args.push_back("-target".to_string());
        args.push_back(ZIG_TRIPLE.to_string());
    }
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
