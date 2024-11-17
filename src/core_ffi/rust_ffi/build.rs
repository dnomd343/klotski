extern crate cmake;

use cxx_build::CFG;

// NOTE: add `CC=clang-20 CXX=clang++-20 CXXFLAGS="-stdlib=libc++"` for cargo command
//       we should keep cxx crate using clang for `cxx.cc`

// NOTE: also, `RUSTFLAGS="-C linker=clang-20"` should be added to cargo env for using lld

// NOTE: add `CC=clang-20 CXX=clang++-20 CXXFLAGS="-stdlib=libc++" RUSTFLAGS="-C linker=clang-20 -C link-arg=-fuse-ld=lld-20 -C link-arg=-stdlib=libc++"` for using llvm toolchain

// NOTE: it seems that cxx_build link `libstdc++` on linux, we can add `-C link-arg=-lc++ -C link-arg=-lc++abi` for workaround, but it should be fixed in the future.

fn main() {
    let dst = cmake::Config::new("klotski")
        .define("CARGO_BUILD:BOOL", "ON")
        .define("KLSK_ENABLE_TESTING:BOOL", "OFF")
        .define("KLSK_ENABLE_BENCHMARK:BOOL", "OFF")
        .define("KLSK_C_FFI:BOOL", "OFF")
        .define("KLSK_PYTHON_FFI:BOOL", "OFF")
        .build();

    CFG.include_prefix = "rust_ffi";

    cxx_build::bridge("src/bridge.rs")
        .file("adapter/short_code.cc")
        .file("adapter/common_code.cc")
        .include("klotski/src/core")
        .flag("-std=c++23")
        .flag("-fno-rtti")
        .flag("-fno-exceptions")
        .compile("klotski");

    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=klotski_core");

    println!("cargo:rerun-if-changed=src/bridge.rs");
    println!("cargo:rerun-if-changed=adapter/short_code.cc");
    println!("cargo:rerun-if-changed=adapter/common_code.cc");
}
