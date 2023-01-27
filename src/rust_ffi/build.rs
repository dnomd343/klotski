extern crate bindgen;

use std::env;
use std::path::PathBuf;

const RELEASE_DIR: &str = "../../cmake-build-release/src/klotski";

fn main() {
    // basic compile options
    println!("cargo:rustc-link-search={}", RELEASE_DIR);
    println!("cargo:rustc-link-lib=static=klotski");
    println!("cargo:rerun-if-changed=wrapper.h");
    println!("cargo:rustc-link-lib=stdc++");

    // build binding info from c-header file
    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    // release binding rust source code
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings.write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings");
}
