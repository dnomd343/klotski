use cxx_build::CFG;

// NOTE: add `CC=clang-20 CXX=clang++-20 CXXFLAGS="-stdlib=libc++"` for cargo command
//       we should keep cxx crate using clang for `cxx.cc`

// NOTE: also, `RUSTFLAGS="-C linker=clang-20"` should be add to cargo env for using lld

fn main() {
    CFG.include_prefix = "rust_ffi";

    // std::env::set_var("CC", "clang-20");
    // std::env::set_var("CXX", "clang++-20");

    cxx_build::bridge("src/common_code.rs")
        .file("adapter/common_code.cc")
        .flag("-std=c++23")
        .flag("-fno-rtti")
        .flag("-fno-exceptions")
        // .flag("-stdlib=libc++")
        .include("../../core")
        // .cpp_set_stdlib("c++")
        // .cpp_link_stdlib("c++")
        .compile("klotski");

    println!("cargo:rustc-link-search=native=../../../cmake-build-release/src/core");
    println!("cargo:rustc-link-lib=static=klotski_core");

    println!("cargo:rustc-link-arg=-fuse-ld=lld-20");
    println!("cargo:rustc-link-arg=-stdlib=libc++");

    println!("cargo:rerun-if-changed=adapter/common_code.cc");
    println!("cargo:rerun-if-changed=src/common_code.rs");
}
