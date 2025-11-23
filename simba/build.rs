use std::{fmt::format, path::PathBuf, str::FromStr};

fn main() {
    let target = std::env::var("TARGET").expect("TARGET environment variable must be specified");
    let host = std::env::var("HOST").expect("TARGET environment variable must be specified");

    // Check if we are cross-compiling
    if target != host {
        let zig_toolchain_distribution_dir = std::env::var("TOOLCHAIN_DIR").expect(concat!(
            "Environment variable 'TOOLCHAIN_DIR' is not set. This should ",
            "be set by the bootstrapper and point to the output zig ",
            "wrappers (though this can be done manually)"
        ));
        let zig_toolchain_distribution_dir: PathBuf = zig_toolchain_distribution_dir.into();

        // println actually sets the variables (for some reason?)
        let linker_var = format!(
            "CARGO_TARGET_{}_LINKER",
            target.replace('-', "_").to_uppercase()
        );
        println!(
            "cargo:rustc-env={}={}",
            &linker_var,
            zig_toolchain_distribution_dir.join("cc").to_str().unwrap()
        );

        println!(
            "cargo:warning=LOOKATME->{}={}",
            &linker_var,
            std::env::var(&linker_var).unwrap_or(String::from("UNSET"))
        );
    }
}
