extern crate cmake;

use cxx_build::CFG;

// NOTE: add `CC=clang-20 CXX=clang++-20 CXXFLAGS="-stdlib=libc++"` for cargo command
//       we should keep cxx crate using clang for `cxx.cc`

// NOTE: also, `RUSTFLAGS="-C linker=clang-20"` should be add to cargo env for using lld

// NOTE: add `CC=clang-20 CXX=clang++-20 CXXFLAGS="-stdlib=libc++" RUSTFLAGS="-C linker=clang-20 -C link-arg=-fuse-ld=lld-20 -C link-arg=-stdlib=libc++"` for using llvm toolchain

fn main() {
    let dst = cmake::Config::new("klotski")
        // .build_target("klotski_core")
        .define("CARGO_BUILD:BOOL", "ON")
        .define("KLSK_ENABLE_TESTING:BOOL", "OFF")
        .define("KLSK_ENABLE_BENCHMARK:BOOL", "OFF")
        .define("KLSK_C_FFI:BOOL", "OFF")
        .define("KLSK_PYTHON_FFI:BOOL", "OFF")
        .build();

    CFG.include_prefix = "rust_ffi";

    cxx_build::bridge("src/common_code.rs")
        .file("adapter/common_code.cc")
        .flag("-std=c++23")
        .flag("-fno-rtti")
        .flag("-fno-exceptions")
        .include("klotski/src/core")
        .compile("klotski");

    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=klotski_core");

    println!("cargo:rerun-if-changed=adapter/common_code.cc");
    println!("cargo:rerun-if-changed=src/common_code.rs");
}
