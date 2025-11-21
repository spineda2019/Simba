fn main() {
    let zig_triple = std::env::var("ZIG_TRIPLE").expect(concat!(
        "ZIG_TRIPLE must be set as an environment variable at ",
        "build time! This is done automatically by the bootstrapper, ",
        "but can also be done manually by platform specific methods."
    ));

    println!("cargo:rustc-env=ZIG_TRIPLE={}", zig_triple);
}
