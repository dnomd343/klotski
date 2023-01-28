#![allow(non_snake_case)]
#![allow(non_camel_case_types)]
#![allow(non_upper_case_globals)]
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

use std::env;
use std::ffi::{c_char, CStr};

#[derive(Debug)]
pub struct Version {
    pub author: String,
    pub system: String,
    pub version: String,
    pub compiler: String,
    pub commit_id: String,
    pub build_time: String,
    pub project_url: String,
}

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

pub fn load_version() -> Version {
    unsafe {
        Version {
            author: get_author().dump(),
            system: get_system_info().dump(),
            version: get_version().dump(),
            compiler: get_compiler_info().dump(),
            commit_id: get_commit_id().dump(),
            build_time: get_build_time().dump(),
            project_url: get_project_url().dump(),
        }
    }
}
