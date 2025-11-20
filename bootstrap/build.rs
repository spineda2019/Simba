fn main() {
    let target = std::env::var("TARGET").expect("TARGET should exist");

    // set this at build time
    println!("cargo:rustc-env=CARGO_BUILD_TARGET={}", target);
}
