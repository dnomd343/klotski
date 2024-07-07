fn main() {
    cxx_build::bridge("src/common_code.rs")
        .file("src/demo.cc")
        .compile("klotski");

    println!("cargo:rerun-if-changed=src/demo.cc");
    println!("cargo:rerun-if-changed=src/common_code.rs");
}
