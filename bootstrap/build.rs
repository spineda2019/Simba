fn main() {
    let target = std::env::var("TARGET").expect("TARGET should exist");

    // set this at build time. Don't use CARGO_BUILD_TARGET or this will
    // poison further builds
    println!("cargo:rustc-env=CARGO_HOST_TARGET={}", target);
}
