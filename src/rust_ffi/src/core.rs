/// Klotski c-style API convert by bindgen.
/// Core module expose these interfaces for abstraction.

pub mod Core {
    #![allow(dead_code)]
    #![allow(non_snake_case)]
    #![allow(non_camel_case_types)]
    #![allow(non_upper_case_globals)]
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

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
}
