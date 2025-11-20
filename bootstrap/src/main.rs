use std::{path::Path, process::Command};

mod target_info {
    pub mod error {
        #[derive(Debug)]
        pub enum TripleError {
            IncorrectTripleFormat,
            UnrecognizedArchitecture(String),
            UnrecognizedOS(String),
            UnrecognizedAbi(String),
            DisallowedTriple,
        }
    }

    #[derive(Debug, PartialEq)]
    pub enum Arch {
        X86,
        X64,
        Arm,
        Aarch64,
    }
    impl Arch {
        pub fn new(raw: &str) -> Result<Arch, error::TripleError> {
            match raw {
                "x86" => Ok(Arch::X86),
                "x86_64" => Ok(Arch::X64),
                "arm" => Ok(Arch::Arm),
                "aarch64" => Ok(Arch::Aarch64),
                other => Err(error::TripleError::UnrecognizedArchitecture(
                    other.to_string(),
                )),
            }
        }
    }

    #[derive(Debug, PartialEq)]
    pub enum Os {
        Linux,
        MacOS,
        Windows,
    }
    impl Os {
        /// 'from_zig' meaning using the zig/clang triple naming convention
        /// (e.g. macos as in aarch64-macos-none) vs what rustup uses (e.g apple
        /// as in aarch64-apple-darwin)
        pub fn from_zig(raw: &str) -> Result<Os, error::TripleError> {
            match raw {
                "linux" => Ok(Os::Linux),
                "macos" => Ok(Os::MacOS),
                "windows" => Ok(Os::Windows),
                other => Err(error::TripleError::UnrecognizedOS(other.to_string())),
            }
        }
    }

    #[derive(Debug, PartialEq)]
    pub enum Abi {
        Gnu,
        /// Linux Only
        Musl,
        /// Should only be used for Mac
        None,
    }
    impl Abi {
        pub fn new(raw: &str) -> Result<Abi, error::TripleError> {
            match raw {
                "gnu" => Ok(Abi::Gnu),
                "musl" => Ok(Abi::Musl),
                "none" => Ok(Abi::None),
                // TODO: MSVC?
                other => Err(error::TripleError::UnrecognizedAbi(other.to_string())),
            }
        }
    }

    pub struct ZigTriple {
        arch: Arch,
        os: Os,
        abi: Abi,
    }

    impl ZigTriple {
        const ALLOWED: [ZigTriple; 13] = [
            ZigTriple {
                arch: Arch::X86,
                os: Os::Windows,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::X64,
                os: Os::Windows,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::Aarch64,
                os: Os::Windows,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::X64,
                os: Os::MacOS,
                abi: Abi::None,
            },
            ZigTriple {
                arch: Arch::Aarch64,
                os: Os::MacOS,
                abi: Abi::None,
            },
            ZigTriple {
                arch: Arch::X86,
                os: Os::Linux,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::X64,
                os: Os::Linux,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::Arm,
                os: Os::Linux,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::Aarch64,
                os: Os::Linux,
                abi: Abi::Gnu,
            },
            ZigTriple {
                arch: Arch::X86,
                os: Os::Linux,
                abi: Abi::Musl,
            },
            ZigTriple {
                arch: Arch::X64,
                os: Os::Linux,
                abi: Abi::Musl,
            },
            ZigTriple {
                arch: Arch::Arm,
                os: Os::Linux,
                abi: Abi::Musl,
            },
            ZigTriple {
                arch: Arch::Aarch64,
                os: Os::Linux,
                abi: Abi::Musl,
            },
        ];

        fn analyze_pieces(pieces: &[&str]) -> Result<ZigTriple, error::TripleError> {
            let raw_target = pieces.join("-");
            println!("Parsing {} ...", raw_target);

            let (arch, os, abi): (Arch, Os, Abi) = match pieces {
                [_, sub_arch, _, _] => {
                    // e.g. x86_64-pc-windows-gnu
                    eprintln!(concat!(
                        "The bootstrapper expects a target triple in the form of ",
                        "arch-os-abi or arch-abi."
                    ));
                    eprintln!(
                        "The presence of '{}' in '{}' implies usage of the rustup format",
                        sub_arch, raw_target
                    );
                    eprintln!("Please use the zig style triple.");
                    eprintln!("The bootstrapper will handle translation for rust.");
                    return Err(error::TripleError::IncorrectTripleFormat);
                }
                [arch, os, abi] => {
                    // e.g. x86_64-windows-gnu
                    let target_arch: Arch = match Arch::new(arch) {
                        Ok(a) => a,
                        Err(error::TripleError::UnrecognizedArchitecture(bad_arch)) => {
                            eprintln!("Unrecognized Arch in triple: {}", bad_arch);
                            return Err(error::TripleError::UnrecognizedArchitecture(bad_arch));
                        }
                        Err(other) => {
                            return Err(other);
                        }
                    };

                    let target_os: Os = match Os::from_zig(os) {
                        Ok(o) => o,
                        Err(error::TripleError::UnrecognizedOS(bad_os)) => {
                            eprintln!("Unrecognized OS in triple: {}", bad_os);
                            eprintln!(concat!(
                                "Ensure you are using the zig naming convention ",
                                "(e.g. macos) rather than the rustup naming convention (e.g. apple)"
                            ));
                            return Err(error::TripleError::UnrecognizedArchitecture(bad_os));
                        }
                        Err(other) => {
                            return Err(other);
                        }
                    };

                    let target_abi: Abi = match Abi::new(abi) {
                        Ok(ab) => ab,
                        Err(error::TripleError::UnrecognizedAbi(bad_abi)) => {
                            eprintln!("Unrecognized abi in triple: {}", bad_abi);
                            eprintln!(concat!(
                                "Ensure you are using the zig naming convention ",
                                "(e.g. none) rather than the rustup naming convention (e.g. darwin)"
                            ));
                            return Err(error::TripleError::UnrecognizedArchitecture(bad_abi));
                        }
                        Err(other) => {
                            return Err(other);
                        }
                    };

                    (target_arch, target_os, target_abi)
                }
                [arch, os] => {
                    // aarch64-macos
                    let target_arch: Arch = match Arch::new(arch) {
                        Ok(a) => a,
                        Err(error::TripleError::UnrecognizedArchitecture(bad_arch)) => {
                            eprintln!("Unrecognized Arch in triple: {}", bad_arch);
                            return Err(error::TripleError::UnrecognizedArchitecture(bad_arch));
                        }
                        Err(other) => {
                            return Err(other);
                        }
                    };

                    let target_os: Os = match Os::from_zig(os) {
                        Ok(o) => o,
                        Err(error::TripleError::UnrecognizedOS(bad_os)) => {
                            eprintln!("Unrecognized OS in triple: {}", bad_os);
                            eprintln!(concat!(
                                "Ensure you are using the zig naming convention ",
                                "(e.g. macos) rather than the rustup naming convention (e.g. apple)"
                            ));
                            return Err(error::TripleError::UnrecognizedArchitecture(bad_os));
                        }
                        Err(other) => {
                            return Err(other);
                        }
                    };

                    let target_abi = match target_os {
                        Os::Linux => Abi::Musl, // default for portability
                        Os::MacOS => Abi::None,
                        Os::Windows => Abi::Gnu,
                    };

                    (target_arch, target_os, target_abi)
                }
                _ => {
                    eprintln!(
                        "target '{}' must have at least an arch and OS tag",
                        raw_target
                    );
                    return Err(error::TripleError::IncorrectTripleFormat);
                }
            };

            let candidate = ZigTriple { arch, os, abi };

            for allowed in &ZigTriple::ALLOWED {
                if allowed.arch == candidate.arch
                    && allowed.os == candidate.os
                    && allowed.abi == candidate.abi
                {
                    return Ok(candidate);
                }
            }

            eprintln!("Valid but disallowed triple encountered: {}", raw_target);

            Err(error::TripleError::DisallowedTriple)
        }

        pub fn new(triple: &str) -> Result<ZigTriple, error::TripleError> {
            let pieces: Vec<&str> = triple.split('-').collect();
            ZigTriple::analyze_pieces(&pieces)
        }

        pub fn native() -> Result<ZigTriple, error::TripleError> {
            let native = env!("CARGO_BUILD_TARGET");
            let pieces: Vec<&str> = native.split('-').collect();
            ZigTriple::analyze_pieces(&pieces)
        }

        pub fn print(&self) {
            println!("Target is the following valid zig-style triple:");
            println!("\tArchitecture: {:?}", self.arch);
            println!("\tOS: {:?}", self.os);
            println!("\tABI: {:?}", self.abi);
            println!();
        }
    }
}

mod arg_parsing {
    use argparse::{ArgumentParser, Store};

    pub fn get_target(
    ) -> Result<crate::target_info::ZigTriple, crate::target_info::error::TripleError> {
        let mut target_str = String::new();

        {
            let mut argument_parser = ArgumentParser::new();
            const DESCRIPTION: &str = "Stub";
            argument_parser.set_description(DESCRIPTION);
            argument_parser.refer(&mut target_str).add_option(
                &["--target"],
                Store,
                "target triple (in zig format)",
            );

            argument_parser.parse_args_or_exit();
        }

        if !target_str.is_empty() {
            crate::target_info::ZigTriple::new(&target_str)
        } else {
            crate::target_info::ZigTriple::native()
        }
    }
}

fn main() -> Result<(), target_info::error::TripleError> {
    let target: target_info::ZigTriple = arg_parsing::get_target()?;
    target.print();

    const CARGO_EXE_PATH: &str = env!("CARGO");
    let bootstrap_package_path: &Path = Path::new(env!("CARGO_MANIFEST_DIR"));
    let workspace_path: &Path = bootstrap_package_path
        .parent()
        .expect("Could not find Workspace path");
    let workspace_path: &str = workspace_path
        .to_str()
        .expect("could not convert Workspace path to str");

    println!(
        "bootstrapping toolchain by calling {} from {}",
        CARGO_EXE_PATH, workspace_path
    );

    let cmd = Command::new(CARGO_EXE_PATH)
        .current_dir(workspace_path)
        .args(["build", "-p", "zig_passthrough", "--release"])
        .output()
        .expect("Failed to build zig bootstrap...");

    match cmd.status.success() {
        true => {
            print!(
                "{}",
                cmd.stdout
                    .iter()
                    .map(|byte| char::from(*byte))
                    .collect::<String>()
            );
        }
        false => {
            eprint!(
                "{}",
                cmd.stderr
                    .iter()
                    .map(|byte| char::from(*byte))
                    .collect::<String>()
            );
            std::process::exit(1);
        }
    }

    Ok(())
}
