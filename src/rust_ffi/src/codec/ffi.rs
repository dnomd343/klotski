use std::ffi::{c_char, CString};
use super::Core;

fn short_code_enable() {
    unsafe {
        Core::short_code_enable()
    }
}

fn short_code_enable_fast() {
    unsafe {
        Core::short_code_enable_fast()
    }
}

fn is_short_code_available() -> bool {
    unsafe {
        Core::is_short_code_available()
    }
}

fn is_short_code_available_fast() -> bool {
    unsafe {
        Core::is_short_code_available_fast()
    }
}

fn raw_code_check(raw_code: u64) -> bool {
    unsafe {
        Core::raw_code_check(raw_code)
    }
}

fn short_code_check(short_code: u32) -> bool {
    unsafe {
        Core::short_code_check(short_code)
    }
}

fn common_code_check(common_code: u64) -> bool {
    unsafe {
        Core::common_code_check(common_code)
    }
}

fn raw_code_to_short_code(raw_code: u64) -> Result<u32, &'static str> {
    let mut short_code: u32 = 0;
    unsafe {
        match Core::raw_code_to_short_code(raw_code, &mut short_code) {
            true => Ok(short_code),
            _ => Err("invalid raw code"),
        }
    }
}

fn short_code_to_raw_code(short_code: u32) -> Result<u64, &'static str> {
    let mut raw_code: u64 = 0;
    unsafe {
        match Core::short_code_to_raw_code(short_code, &mut raw_code) {
            true => Ok(raw_code),
            _ => Err("invalid short code"),
        }
    }
}

fn raw_code_to_common_code(raw_code: u64) -> Result<u64, &'static str> {
    let mut common_code: u64 = 0;
    unsafe {
        match Core::raw_code_to_common_code(raw_code, &mut common_code) {
            true => Ok(common_code),
            _ => Err("invalid raw code"),
        }
    }
}

fn common_code_to_raw_code(common_code: u64) -> Result<u64, &'static str> {
    let mut raw_code: u64 = 0;
    unsafe {
        match Core::common_code_to_raw_code(common_code, &mut raw_code) {
            true => Ok(raw_code),
            _ => Err("invalid common code"),
        }
    }
}

fn short_code_to_common_code(short_code: u32) -> Result<u64, &'static str> {
    let mut common_code: u64 = 0;
    unsafe {
        match Core::short_code_to_common_code(short_code, &mut common_code) {
            true => Ok(common_code),
            _ => Err("invalid short code"),
        }
    }
}

fn common_code_to_short_code(common_code: u64) -> Result<u32, &'static str> {
    let mut short_code: u32 = 0;
    unsafe {
        match Core::common_code_to_short_code(common_code, &mut short_code) {
            true => Ok(short_code),
            _ => Err("invalid common code"),
        }
    }
}

fn raw_code_to_short_code_unsafe(raw_code: u64) -> u32 {
    unsafe {
        Core::raw_code_to_short_code_unsafe(raw_code)
    }
}

fn short_code_to_raw_code_unsafe(short_code: u32) -> u64 {
    unsafe {
        Core::short_code_to_raw_code_unsafe(short_code)
    }
}

fn raw_code_to_common_code_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::raw_code_to_common_code_unsafe(raw_code)
    }
}

fn common_code_to_raw_code_unsafe(common_code: u64) -> u64 {
    unsafe {
        Core::common_code_to_raw_code_unsafe(common_code)
    }
}

fn short_code_to_common_code_unsafe(short_code: u32) -> u64 {
    unsafe {
        Core::short_code_to_common_code_unsafe(short_code)
    }
}

fn common_code_to_short_code_unsafe(common_code: u64) -> u32 {
    unsafe {
        Core::common_code_to_short_code_unsafe(common_code)
    }
}

fn is_vertical_mirror(raw_code: u64) -> Result<bool, &'static str> {
    let mut result: bool = false;
    unsafe {
        match Core::is_vertical_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

fn is_horizontal_mirror(raw_code: u64) -> Result<bool, &'static str> {
    let mut result: bool = false;
    unsafe {
        match Core::is_horizontal_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

fn to_vertical_mirror(raw_code: u64) -> Result<u64, &'static str> {
    let mut result: u64 = 0;
    unsafe {
        match Core::to_vertical_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

fn to_horizontal_mirror(raw_code: u64) -> Result<u64, &'static str> {
    let mut result: u64 = 0;
    unsafe {
        match Core::to_horizontal_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

fn is_vertical_mirror_unsafe(raw_code: u64) -> bool {
    unsafe {
        Core::is_vertical_mirror_unsafe(raw_code)
    }
}

fn is_horizontal_mirror_unsafe(raw_code: u64) -> bool {
    unsafe {
        Core::is_horizontal_mirror_unsafe(raw_code)
    }
}

fn to_vertical_mirror_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::to_vertical_mirror_unsafe(raw_code)
    }
}

fn to_horizontal_mirror_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::to_horizontal_mirror_unsafe(raw_code)
    }
}

fn short_code_to_string(short_code: u32) -> Result<String, &'static str> {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::SHORT_CODE_STR_SIZE as usize];
        match Core::short_code_to_string(short_code, buffer.as_mut_ptr()) {
            true => {
                let mut result = String::new();
                for c in &buffer[..buffer.len() - 1] {
                    result.push(*c as u8 as char);
                }
                Ok(result)
            },
            _ => Err("invalid short code"),
        }
    }
}

fn short_code_to_string_unsafe(short_code: u32) -> String {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::SHORT_CODE_STR_SIZE as usize];
        Core::short_code_to_string(short_code, buffer.as_mut_ptr());
        let mut result = String::new();
        for c in &buffer[..buffer.len() - 1] {
            result.push(*c as u8 as char);
        }
        result
    }
}

fn short_code_from_string(short_code: &str) -> Result<u32, &'static str> {
    unsafe {
        let mut result: u32 = 0;
        match Core::short_code_from_string(
            CString::new(short_code).unwrap().into_raw(),
            &mut result
        ) {
            true => Ok(result),
            _ => Err("invalid short code text"),
        }
    }
}

fn common_code_to_string(common_code: u64) -> Result<String, &'static str> {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::COMMON_CODE_STR_SIZE as usize];
        match Core::common_code_to_string(common_code, buffer.as_mut_ptr()) {
            true => {
                let mut result = String::new();
                for c in &buffer[..buffer.len() - 1] {
                    result.push(*c as u8 as char);
                }
                Ok(result)
            },
            _ => Err("invalid common code"),
        }
    }
}

fn common_code_to_string_unsafe(common_code: u64) -> String {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::COMMON_CODE_STR_SIZE as usize];
        Core::common_code_to_string_unsafe(common_code, buffer.as_mut_ptr());
        let mut result = String::new();
        for c in &buffer[..buffer.len() - 1] {
            result.push(*c as u8 as char);
        }
        result
    }
}

fn common_code_to_string_shorten(common_code: u64) -> Result<String, &'static str> {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::COMMON_CODE_STR_SIZE as usize];
        match Core::common_code_to_string_shorten(common_code, buffer.as_mut_ptr()) {
            true => {
                let mut result = String::new();
                for c in &buffer[..buffer.len() - 1] {
                    if *c == 0 { break; }
                    result.push(*c as u8 as char);
                }
                Ok(result)
            },
            _ => Err("invalid common code"),
        }
    }
}

fn common_code_to_string_shorten_unsafe(common_code: u64) -> String {
    unsafe {
        let mut buffer: Vec<c_char> = vec![0; Core::COMMON_CODE_STR_SIZE as usize];
        Core::common_code_to_string_shorten(common_code, buffer.as_mut_ptr());
        let mut result = String::new();
        for c in &buffer[..buffer.len() - 1] {
            if *c == 0 { break; }
            result.push(*c as u8 as char);
        }
        result
    }
}

fn common_code_from_string(common_code: &str) -> Result<u64, &'static str> {
    unsafe {
        let mut result: u64 = 0;
        match Core::common_code_from_string(
            CString::new(common_code).unwrap().into_raw(),
            &mut result
        ) {
            true => Ok(result),
            _ => Err("invalid common code text"),
        }
    }
}
