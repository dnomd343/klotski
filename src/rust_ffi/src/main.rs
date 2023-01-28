#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::env;
use std::ffi::CStr;

fn klotski_tmain() {
    unsafe {
        tmain();
    }
}

fn klotski_version() -> String {
    unsafe {
        // let v_major = get_version_major();

        let v = get_version();
        let csp: &CStr = CStr::from_ptr(v);
        let s: &str = csp.to_str().unwrap();
        // println!("klotski version -> {}", s);

        String::from(s)
        // String::new()
    }
}

fn main() {
    println!("rust start");

    // klotski_tmain();

    println!("version -> {}", klotski_version());

    println!("rust exit");
}
