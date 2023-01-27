#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::env;

fn klotski_tmain() {
    unsafe {
        tmain();
    }
}

fn main() {
    println!("rust start");
    klotski_tmain();
    println!("rust exit");
}
