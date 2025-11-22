use std::path::PathBuf;

fn main() {
    // these are provided by cargo since zig_passthrough is a binary dependency.
    let target = std::env::var("TARGET").expect("TARGET environment variable must be specified");
    // e.g., X86_64_PC_WINDOWS_GNU
    let target_var_name = target.replace('-', "_").to_uppercase();
    let zig_toolchain_distribution_dir = std::env::var("TOOLCHAIN_DIR").expect(concat!(
        "Environment variable 'TOOLCHAIN_DIR' is not set. This should ",
        "be set by the bootstrapper and point to the output zig ",
        "wrappers (though this can be done manually)"
    ));
    let zig_toolchain_distribution_dir: PathBuf = zig_toolchain_distribution_dir.into();

    // println actually sets the variables (for some reason?)
    println!(
        "cargo:rustc-env=CARGO_TARGET_{}_LINKER={}",
        target_var_name,
        zig_toolchain_distribution_dir.join("cc").to_str().unwrap()
    );
}
