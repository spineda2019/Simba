fn main() {
    // these are provided by cargo since zig_passthrough is a binary dependency.
    let target = std::env::var("TARGET").expect("TARGET environment variable must be specified");

    // cargo environment variables expect SNAKE_CASE for configuration
    // e.g., X86_64_PC_WINDOWS_GNU
    let target_var_name = target.replace('-', "_").to_uppercase();

    dbg!(std::env::vars());
    // use zig for everything C style tools are needed. This will make cross compiling a breeze
    // Environment variable for a path to a build dependent tool will bin the the form of
    // CARGO_BIN_FILE_<PACKAGE_NAME>_<BIN_NAME>, where everything is all caps except
    // BIN_NAME, which is case sensitive. e.g:
    // CARGO_BIN_FILE_ZIG_PASSTHROUGH_cxx
    let linker_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_cc")
        .expect("Could not get output path for built linker");
    let ar_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_ar")
        .expect("Could not get output path for built archiver");
    let cc_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_cc")
        .expect("Could not get output path for built cc");
    let cxx_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_cxx")
        .expect("Could not get output path for built cxx");
    let objcopy_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_objcopy")
        .expect("Could not get output path for built objcopy");
    let rc_path = std::env::var("CARGO_BIN_FILE_ZIG_PASSTHROUGH_rc")
        .expect("Could not get output path for built rc");

    // println actually sets the variables (for some reason?)
    println!(
        "cargo:rustc-env=CARGO_TARGET_{}_LINKER={}",
        target_var_name, linker_path
    );
    println!(
        "cargo:rustc-env=CARGO_TARGET_{}_AR={}",
        target_var_name, ar_path
    );
    println!("cargo:rustc-env=CC={}", cc_path);
    println!("cargo:rustc-env=CXX={}", cxx_path);
    println!("cargo:rustc-env=CMAKE_C_COMPILER={}", cc_path);
    println!("cargo:rustc-env=CMAKE_CXX_COMPILER={}", cxx_path);
    println!("cargo:rustc-env=AR={}", ar_path);
    println!(
        "cargo:rustc-env=CARGO_TARGET_{}_OBJCOPY={}",
        target_var_name, objcopy_path
    );
    println!(
        "cargo:rustc-env=CARGO_TARGET_{}_WINDRES={}",
        target_var_name, rc_path
    );
}
