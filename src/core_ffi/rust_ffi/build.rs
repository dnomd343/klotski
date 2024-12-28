// NOTE: add the following environment variables for using llvm toolchain
//         CC=clang-20
//         CXX=clang++-20
//         CXXSTDLIB="c++"
//         CXXFLAGS="-stdlib=libc++"
//         RUSTFLAGS="-Clinker=clang-20 -Clink-arg=-fuse-ld=lld-20 -Clink-arg=-stdlib=libc++"

use cxx_build::CFG;

// TODO: assert version info

fn main() {
    if std::env::var("DOCS_RS").is_err() {
        cmake::Config::new("klotski")
            .define("KLSK_ENABLE_LTO:BOOL", "ON")
            .build();
        println!("cargo:rustc-link-lib=static=klotski_core");

        CFG.include_prefix = "rust_ffi";
        cxx_build::bridge("src/bridge.rs")
            .file("adapter/layout.cc")
            .file("adapter/short_code.cc")
            .include("klotski/src/core")
            .flag("-std=c++23")
            .flag_if_supported("-flto=full") // only for clang
            .flag("-fno-rtti")
            .flag("-fno-exceptions")
            .compile("klotski");
    }

    println!("cargo:rerun-if-changed=src/bridge.rs");
    println!("cargo:rerun-if-changed=adapter/layout.cc");
    println!("cargo:rerun-if-changed=adapter/short_code.cc");
}
