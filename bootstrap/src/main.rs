use std::{
    path::{Path, PathBuf},
    process::Command,
    str::FromStr,
};

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
    impl std::fmt::Display for Arch {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Arch::X86 => write!(f, "x86"),
                Arch::X64 => write!(f, "x86_64"),
                Arch::Arm => write!(f, "arm"),
                Arch::Aarch64 => write!(f, "aarch64"),
            }
        }
    }
    pub enum RustArch {
        I686,
        X64,
        Arm,
        Aarch64,
    }
    impl std::fmt::Display for RustArch {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Self::I686 => write!(f, "i686"),
                Self::X64 => write!(f, "x86_64"),
                Self::Arm => write!(f, "arm"),
                Self::Aarch64 => write!(f, "aarch64"),
            }
        }
    }
    pub enum RustVendor {
        Pc,
        Apple,
        Unknown,
    }
    impl std::fmt::Display for RustVendor {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Self::Pc => write!(f, "pc"),
                Self::Apple => write!(f, "apple"),
                Self::Unknown => write!(f, "unknown"),
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
    impl std::fmt::Display for Os {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Os::Linux => write!(f, "linux"),
                Os::Windows => write!(f, "windows"),
                Os::MacOS => write!(f, "macos"),
            }
        }
    }
    pub enum RustOs {
        Linux,
        Darwin,
        Windows,
    }
    impl std::fmt::Display for RustOs {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Self::Linux => write!(f, "linux"),
                Self::Darwin => write!(f, "darwin"),
                Self::Windows => write!(f, "windows"),
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
    impl std::fmt::Display for Abi {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Abi::Gnu => write!(f, "gnu"),
                Abi::Musl => write!(f, "musl"),
                Abi::None => write!(f, "none"),
            }
        }
    }
    pub enum RustAbi {
        Gnu,
        Musl,
        /// gnu abi is specified differently between ARM and x86 on windows.
        /// e.g x86_64-pc-windows-gnu vs aarch64-pc-windows-gnullvm
        GnuLlvm,
    }
    impl std::fmt::Display for RustAbi {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Self::Gnu => write!(f, "gnu"),
                Self::Musl => write!(f, "musl"),
                Self::GnuLlvm => write!(f, "gnullvm"),
            }
        }
    }

    pub struct ZigTriple {
        arch: Arch,
        os: Os,
        abi: Abi,
    }

    /// Frustratingly, rust targets look a bit different than zig triples (which are a bit more
    /// intuitive IMO) so we have to represent them differently
    pub struct RustTarget {
        arch: RustArch,
        vendor: RustVendor,
        os: RustOs,
        abi: Option<RustAbi>,
    }
    impl RustTarget {
        pub fn str(&self) -> String {
            if let Some(abi) = &self.abi {
                format!("{}-{}-{}-{}", self.arch, self.vendor, self.os, abi)
            } else {
                format!("{}-{}-{}", self.arch, self.vendor, self.os)
            }
        }

        pub fn print(&self) {
            println!("Target maps to the following valid rust-style triple:");
            println!("\tArchitecture: {}", self.arch);
            println!("\tVendor: {}", self.vendor);
            println!("\tOS: {}", self.os);
            if let Some(abi) = &self.abi {
                println!("\tABI: {}", abi);
            }
            println!();
        }
    }

    impl ZigTriple {
        /// Exhaustive list of what we'll allow (and how they map to rust targets)
        const ALLOWED: [(ZigTriple, RustTarget); 13] = [
            (
                ZigTriple {
                    arch: Arch::X86,
                    os: Os::Windows,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::I686,
                    vendor: RustVendor::Pc,
                    os: RustOs::Windows,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X64,
                    os: Os::Windows,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::X64,
                    vendor: RustVendor::Pc,
                    os: RustOs::Windows,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Aarch64,
                    os: Os::Windows,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::Aarch64,
                    vendor: RustVendor::Pc,
                    os: RustOs::Windows,
                    abi: Some(RustAbi::GnuLlvm),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X64,
                    os: Os::MacOS,
                    abi: Abi::None,
                },
                RustTarget {
                    arch: RustArch::X64,
                    vendor: RustVendor::Apple,
                    os: RustOs::Darwin,
                    abi: None,
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Aarch64,
                    os: Os::MacOS,
                    abi: Abi::None,
                },
                RustTarget {
                    arch: RustArch::Aarch64,
                    vendor: RustVendor::Apple,
                    os: RustOs::Darwin,
                    abi: None,
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X86,
                    os: Os::Linux,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::I686,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X64,
                    os: Os::Linux,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::X64,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Arm,
                    os: Os::Linux,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::Arm,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Aarch64,
                    os: Os::Linux,
                    abi: Abi::Gnu,
                },
                RustTarget {
                    arch: RustArch::Aarch64,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Gnu),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X86,
                    os: Os::Linux,
                    abi: Abi::Musl,
                },
                RustTarget {
                    arch: RustArch::I686,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Musl),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::X64,
                    os: Os::Linux,
                    abi: Abi::Musl,
                },
                RustTarget {
                    arch: RustArch::X64,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Musl),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Arm,
                    os: Os::Linux,
                    abi: Abi::Musl,
                },
                RustTarget {
                    arch: RustArch::Arm,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Musl),
                },
            ),
            (
                ZigTriple {
                    arch: Arch::Aarch64,
                    os: Os::Linux,
                    abi: Abi::Musl,
                },
                RustTarget {
                    arch: RustArch::Aarch64,
                    vendor: RustVendor::Unknown,
                    os: RustOs::Linux,
                    abi: Some(RustAbi::Musl),
                },
            ),
        ];

        fn analyze_pieces(pieces: &[&str]) -> Result<(ZigTriple, RustTarget), error::TripleError> {
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

            for (allowed, rust_mapping) in ZigTriple::ALLOWED {
                if allowed.arch == candidate.arch
                    && allowed.os == candidate.os
                    && allowed.abi == candidate.abi
                {
                    return Ok((allowed, rust_mapping));
                }
            }

            eprintln!("Valid but disallowed triple encountered: {}", raw_target);

            Err(error::TripleError::DisallowedTriple)
        }

        pub fn new(triple: &str) -> Result<(ZigTriple, RustTarget), error::TripleError> {
            let pieces: Vec<&str> = triple.split('-').collect();
            ZigTriple::analyze_pieces(&pieces)
        }

        pub fn native() -> Result<(ZigTriple, RustTarget), error::TripleError> {
            let native = env!("CARGO_HOST_TARGET");
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

        pub fn str(&self) -> String {
            format!("{}-{}-{}", self.arch, self.os, self.abi)
        }
    }
}

mod arg_parsing {
    use argparse::{ArgumentParser, Store};

    pub fn get_target() -> Result<
        (
            crate::target_info::ZigTriple,
            crate::target_info::RustTarget,
        ),
        crate::target_info::error::TripleError,
    > {
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

struct CargoRunner {
    cargo_exe_path: &'static str,
    toolchain_output_root: &'static str,
}

impl CargoRunner {
    pub fn new(_cargo_exe_path: &'static str, _toolchain_output_root: &'static str) -> Self {
        /*
        Self {
            cargo_exe_path,
            toolchain_output_root,
        }
        */
        todo!()
    }

    pub fn init_directories(&self) {
        todo!()
    }

    pub fn build() {
        todo!()
    }
}

fn main() -> Result<(), target_info::error::TripleError> {
    let (target, mapped_rust_target): (target_info::ZigTriple, target_info::RustTarget) =
        arg_parsing::get_target()?;
    target.print();
    mapped_rust_target.print();

    const CARGO_EXE_PATH: &str = env!("CARGO");
    const TOOLCHAIN_OUT: &str = "zig_toolchain";

    // let cargo_runner: CargoRunner = CargoRunner::new(CARGO_EXE_PATH, TOOLCHAIN_OUT);

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
        .env("ZIG_TRIPLE", target.str()) // will be available to the passthrough package's build.rs
        .args([
            "build",
            "-p",
            "zig_passthrough",
            "--release",
            "--target-dir",
            TOOLCHAIN_OUT,
        ])
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

    let native = env!("CARGO_HOST_TARGET");

    let workspace_dir = PathBuf::from_str(workspace_path).unwrap();

    let toolchain_build_dir = PathBuf::from_str(workspace_path)
        .unwrap()
        .join(TOOLCHAIN_OUT)
        .join("release");

    let toolchain_distribution_dir = PathBuf::from_str(workspace_path)
        .unwrap()
        .join(TOOLCHAIN_OUT)
        .join("dist");

    let toolchain_destination_dir = PathBuf::from_str(workspace_path)
        .unwrap()
        .join(TOOLCHAIN_OUT)
        .join("dist")
        .join(target.str());

    if !toolchain_destination_dir.exists() {
        if !toolchain_distribution_dir.exists() {
            std::fs::create_dir(&toolchain_distribution_dir).unwrap();
        }
        std::fs::create_dir(&toolchain_destination_dir).unwrap();
    }

    dbg!(&toolchain_build_dir);
    for entry in std::fs::read_dir(&toolchain_build_dir).unwrap() {
        let entry = entry.unwrap();
        let file_type = entry.file_type().unwrap();

        let path = entry.path();

        // Construct the corresponding destination path
        // We use file_name() to get just the last component (e.g., "file.txt")
        // and push it onto the destination root.
        let dest_path = toolchain_destination_dir.join(entry.file_name());

        if !file_type.is_dir() {
            let _ = std::fs::copy(&path, &dest_path);
        }
    }

    println!(
        "Placing bootstrapped tools from {} in {}",
        toolchain_build_dir.to_str().unwrap(),
        toolchain_destination_dir.to_str().unwrap()
    );

    println!("Using bootstrapped zig passthrough to build project");
    println!(
        "Passing follwoing target to rust: {}",
        &mapped_rust_target.str()
    );
    println!();

    let cmd = Command::new(CARGO_EXE_PATH)
        .current_dir(workspace_path)
        .env(
            "CMAKE_TOOLCHAIN_FILE",
            workspace_dir.join("toolchain.cmake"),
        )
        .env("TOOLCHAIN_DIR", toolchain_destination_dir)
        .args([
            "build",
            "-p",
            "simba",
            "--release",
            "--target",
            &mapped_rust_target.str(),
        ])
        .output()
        .expect("Failed to build simba...");

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
