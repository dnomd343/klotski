#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::env;
use std::ffi::{c_char, CStr};

pub trait CStringDump {
    fn dump(&self) -> String;
}

impl CStringDump for *const c_char {
    fn dump(&self) -> String {
        let cstr: &CStr;
        unsafe {
            cstr = CStr::from_ptr(*self);
        }
        String::from(cstr.to_str().unwrap())
    }
}

// fn klotski_tmain() {
//     unsafe {
//         tmain();
//     }
// }

// fn const_char_dump(i: *const c_char) {
//
// }

fn klotski_version() -> String {
    unsafe {
        // let v_major = get_version_major();

        // let v = get_version();
        // let csp: &CStr = CStr::from_ptr(v);
        // let s: &str = csp.to_str().unwrap();
        // println!("klotski version -> {}", s);

        String::from(get_version().dump())
        // String::new()
    }
}

fn klotski_system() -> String {
    unsafe {
        String::from(get_system_info().dump())
    }
}

fn main() {
    println!("rust start");

    // klotski_tmain();

    println!("version -> {}", klotski_version());
    println!("system -> {}", klotski_system());

    println!("rust exit");
}
