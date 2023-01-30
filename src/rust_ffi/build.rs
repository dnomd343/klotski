extern crate bindgen;

use std::env;
use std::path::PathBuf;

const KLOTSKI_LIB: &str = "klotski";
const KLOTSKI_HEADER: &str = "klotski.h";
const KLOTSKI_RUST_ABI: &str = "bindings.rs";
const KLOTSKI_RELEASE_DIR: &str = "../../bin/";

fn main() {
    // path of klotski c-header file
    let header_path = PathBuf::from(KLOTSKI_RELEASE_DIR).join(KLOTSKI_HEADER);
    let klotski_c_header = header_path.to_str().unwrap();

    // basic cargo compile options
    println!("cargo:rustc-link-search={}", KLOTSKI_RELEASE_DIR);
    println!("cargo:rustc-link-lib=static={}", KLOTSKI_LIB);
    println!("cargo:rerun-if-changed={}", klotski_c_header);
    println!("cargo:rustc-link-lib=stdc++");

    // build binding info from c-header file
    let bindings = bindgen::Builder::default()
        .header(klotski_c_header)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    // release binding rust source code
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings.write_to_file(out_path.join(KLOTSKI_RUST_ABI))
        .expect("Couldn't write bindings");
}
