use crate::core::Core;
use std::ffi::{c_char, CString};

#[inline]
pub fn short_code_enable() {
    unsafe {
        Core::short_code_enable()
    }
}

#[inline]
pub fn short_code_enable_fast() {
    unsafe {
        Core::short_code_enable_fast()
    }
}

#[inline]
#[allow(dead_code)]
pub fn is_short_code_available() -> bool {
    unsafe {
        Core::is_short_code_available()
    }
}

#[inline]
#[allow(dead_code)]
pub fn is_short_code_available_fast() -> bool {
    unsafe {
        Core::is_short_code_available_fast()
    }
}

#[inline]
pub fn raw_code_check(raw_code: u64) -> bool {
    unsafe {
        Core::raw_code_check(raw_code)
    }
}

#[inline]
pub fn short_code_check(short_code: u32) -> bool {
    unsafe {
        Core::short_code_check(short_code)
    }
}

#[inline]
pub fn common_code_check(common_code: u64) -> bool {
    unsafe {
        Core::common_code_check(common_code)
    }
}

// #[allow(dead_code)]
pub fn raw_code_to_short_code(raw_code: u64) -> Result<u32, &'static str> {
    let mut short_code: u32 = 0;
    unsafe {
        match Core::raw_code_to_short_code(raw_code, &mut short_code) {
            true => Ok(short_code),
            _ => Err("invalid raw code"),
        }
    }
}

// #[allow(dead_code)]
pub fn short_code_to_raw_code(short_code: u32) -> Result<u64, &'static str> {
    let mut raw_code: u64 = 0;
    unsafe {
        match Core::short_code_to_raw_code(short_code, &mut raw_code) {
            true => Ok(raw_code),
            _ => Err("invalid short code"),
        }
    }
}

// #[allow(dead_code)]
pub fn raw_code_to_common_code(raw_code: u64) -> Result<u64, &'static str> {
    let mut common_code: u64 = 0;
    unsafe {
        match Core::raw_code_to_common_code(raw_code, &mut common_code) {
            true => Ok(common_code),
            _ => Err("invalid raw code"),
        }
    }
}

// #[allow(dead_code)]
pub fn common_code_to_raw_code(common_code: u64) -> Result<u64, &'static str> {
    let mut raw_code: u64 = 0;
    unsafe {
        match Core::common_code_to_raw_code(common_code, &mut raw_code) {
            true => Ok(raw_code),
            _ => Err("invalid common code"),
        }
    }
}

// #[allow(dead_code)]
pub fn short_code_to_common_code(short_code: u32) -> Result<u64, &'static str> {
    let mut common_code: u64 = 0;
    unsafe {
        match Core::short_code_to_common_code(short_code, &mut common_code) {
            true => Ok(common_code),
            _ => Err("invalid short code"),
        }
    }
}

// #[allow(dead_code)]
pub fn common_code_to_short_code(common_code: u64) -> Result<u32, &'static str> {
    let mut short_code: u32 = 0;
    unsafe {
        match Core::common_code_to_short_code(common_code, &mut short_code) {
            true => Ok(short_code),
            _ => Err("invalid common code"),
        }
    }
}

#[allow(dead_code)]
pub fn raw_code_to_short_code_unsafe(raw_code: u64) -> u32 {
    unsafe {
        Core::raw_code_to_short_code_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn short_code_to_raw_code_unsafe(short_code: u32) -> u64 {
    unsafe {
        Core::short_code_to_raw_code_unsafe(short_code)
    }
}

#[allow(dead_code)]
pub fn raw_code_to_common_code_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::raw_code_to_common_code_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn common_code_to_raw_code_unsafe(common_code: u64) -> u64 {
    unsafe {
        Core::common_code_to_raw_code_unsafe(common_code)
    }
}

#[allow(dead_code)]
pub fn short_code_to_common_code_unsafe(short_code: u32) -> u64 {
    unsafe {
        Core::short_code_to_common_code_unsafe(short_code)
    }
}

#[allow(dead_code)]
pub fn common_code_to_short_code_unsafe(common_code: u64) -> u32 {
    unsafe {
        Core::common_code_to_short_code_unsafe(common_code)
    }
}

#[allow(dead_code)]
pub fn is_vertical_mirror(raw_code: u64) -> Result<bool, &'static str> {
    let mut result: bool = false;
    unsafe {
        match Core::is_vertical_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

#[allow(dead_code)]
pub fn is_horizontal_mirror(raw_code: u64) -> Result<bool, &'static str> {
    let mut result: bool = false;
    unsafe {
        match Core::is_horizontal_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

#[allow(dead_code)]
pub fn to_vertical_mirror(raw_code: u64) -> Result<u64, &'static str> {
    let mut result: u64 = 0;
    unsafe {
        match Core::to_vertical_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

#[allow(dead_code)]
pub fn to_horizontal_mirror(raw_code: u64) -> Result<u64, &'static str> {
    let mut result: u64 = 0;
    unsafe {
        match Core::to_horizontal_mirror(raw_code, &mut result) {
            true => Ok(result),
            _ => Err("invalid raw code"),
        }
    }
}

#[allow(dead_code)]
pub fn is_vertical_mirror_unsafe(raw_code: u64) -> bool {
    unsafe {
        Core::is_vertical_mirror_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn is_horizontal_mirror_unsafe(raw_code: u64) -> bool {
    unsafe {
        Core::is_horizontal_mirror_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn to_vertical_mirror_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::to_vertical_mirror_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn to_horizontal_mirror_unsafe(raw_code: u64) -> u64 {
    unsafe {
        Core::to_horizontal_mirror_unsafe(raw_code)
    }
}

#[allow(dead_code)]
pub fn short_code_to_string(short_code: u32) -> Result<String, &'static str> {
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

#[allow(dead_code)]
pub fn short_code_to_string_unsafe(short_code: u32) -> String {
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

#[allow(dead_code)]
pub fn short_code_from_string(short_code: &str) -> Result<u32, &'static str> {
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

#[allow(dead_code)]
pub fn common_code_to_string(common_code: u64) -> Result<String, &'static str> {
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

#[allow(dead_code)]
pub fn common_code_to_string_unsafe(common_code: u64) -> String {
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

#[allow(dead_code)]
pub fn common_code_to_string_shorten(common_code: u64) -> Result<String, &'static str> {
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

#[allow(dead_code)]
pub fn common_code_to_string_shorten_unsafe(common_code: u64) -> String {
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

#[allow(dead_code)]
pub fn common_code_from_string(common_code: &str) -> Result<u64, &'static str> {
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn warm_up() {
        assert_eq!(is_short_code_available(), false);
        short_code_enable();
        assert_eq!(is_short_code_available(), true);

        assert_eq!(is_short_code_available_fast(), false);
        short_code_enable_fast();
        assert_eq!(is_short_code_available_fast(), true);
    }

    const TEST_SHORT_CODE_OK: u32 = 4091296;
    const TEST_SHORT_CODE_STR_OK: &str = "4WVE1";
    const TEST_COMMON_CODE_OK: u64 = 0x1_A9BF_0C00;
    const TEST_COMMON_CODE_STR_OK: &str = "1A9BF0C00";
    const TEST_COMMON_CODE_STR_SHR_OK: &str = "1A9BF0C";
    const TEST_RAW_CODE_OK: u64 = 0x0603_EDF5_CAFF_F5E2;

    const TEST_SHORT_CODE_ERR: u32 = 29670987;
    const TEST_COMMON_CODE_ERR: u64 = 0x1_2190_2300;
    const TEST_RAW_CODE_ERR: u64 = 0x0A34_182B_3810_2D21;

    const TEST_SHORT_CODE_STR_ERR: &str = "R50EH";
    const TEST_COMMON_CODE_STR_ERR: &str = "123J432A9";

    const TEST_MIRROR_V: u64 = 0x0_FC0_480_6DB_FC0_480;
    const TEST_MIRROR_V1: u64 = 0x0_E58_FC8_5FF_EBC_4DB;
    const TEST_MIRROR_V2: u64 = 0x0_EDB_5FF_EBC_5C8_E58;

    const TEST_MIRROR_H: u64 = 0x0_603_EDF_5CA_FFF_5E2;
    const TEST_MIRROR_H1: u64 = 0x0_E58_FC8_5FF_EBC_4DB;
    const TEST_MIRROR_H2: u64 = 0x0_0F9_1CF_FFA_F17_6DA;

    #[test]
    fn checker() {
        assert!(raw_code_check(TEST_RAW_CODE_OK));
        assert!(!raw_code_check(TEST_RAW_CODE_ERR));

        assert!(short_code_check(TEST_SHORT_CODE_OK));
        assert!(!short_code_check(TEST_SHORT_CODE_ERR));

        assert!(common_code_check(TEST_COMMON_CODE_OK));
        assert!(!common_code_check(TEST_COMMON_CODE_ERR));
    }

    #[test]
    fn convert() {
        // raw code -> short code
        assert!(raw_code_to_short_code(TEST_RAW_CODE_ERR).is_err());
        assert!(raw_code_to_short_code(TEST_RAW_CODE_OK).is_ok());
        assert_eq!(
            raw_code_to_short_code(TEST_RAW_CODE_OK).unwrap(),
            TEST_SHORT_CODE_OK
        );

        // short code -> raw code
        assert!(short_code_to_raw_code(TEST_SHORT_CODE_ERR).is_err());
        assert!(short_code_to_raw_code(TEST_SHORT_CODE_OK).is_ok());
        assert_eq!(
            short_code_to_raw_code(TEST_SHORT_CODE_OK).unwrap(),
            TEST_RAW_CODE_OK
        );

        // raw code -> common code
        assert!(raw_code_to_common_code(TEST_RAW_CODE_ERR).is_err());
        assert!(raw_code_to_common_code(TEST_RAW_CODE_OK).is_ok());
        assert_eq!(
            raw_code_to_common_code(TEST_RAW_CODE_OK).unwrap(),
            TEST_COMMON_CODE_OK
        );

        // common code -> raw code
        assert!(common_code_to_raw_code(TEST_COMMON_CODE_ERR).is_err());
        assert!(common_code_to_raw_code(TEST_COMMON_CODE_OK).is_ok());
        assert_eq!(
            common_code_to_raw_code(TEST_COMMON_CODE_OK).unwrap(),
            TEST_RAW_CODE_OK
        );

        // short code -> common code
        assert!(short_code_to_common_code(TEST_SHORT_CODE_ERR).is_err());
        assert!(short_code_to_common_code(TEST_SHORT_CODE_OK).is_ok());
        assert_eq!(
            short_code_to_common_code(TEST_SHORT_CODE_OK).unwrap(),
            TEST_COMMON_CODE_OK
        );

        // common code -> short code
        assert!(common_code_to_short_code(TEST_COMMON_CODE_ERR).is_err());
        assert!(common_code_to_short_code(TEST_COMMON_CODE_OK).is_ok());
        assert_eq!(
            common_code_to_short_code(TEST_COMMON_CODE_OK).unwrap(),
            TEST_SHORT_CODE_OK
        );
    }

    #[test]
    fn convert_unsafe() {
        // raw code <---> short code
        assert_eq!(
            raw_code_to_short_code_unsafe(TEST_RAW_CODE_OK),
            TEST_SHORT_CODE_OK
        );
        assert_eq!(
            short_code_to_raw_code_unsafe(TEST_SHORT_CODE_OK),
            TEST_RAW_CODE_OK
        );

        // raw code <---> common code
        assert_eq!(
            raw_code_to_common_code_unsafe(TEST_RAW_CODE_OK),
            TEST_COMMON_CODE_OK
        );
        assert_eq!(
            common_code_to_raw_code_unsafe(TEST_COMMON_CODE_OK),
            TEST_RAW_CODE_OK
        );

        // short code <---> common code
        assert_eq!(
            short_code_to_common_code_unsafe(TEST_SHORT_CODE_OK),
            TEST_COMMON_CODE_OK
        );
        assert_eq!(
            common_code_to_short_code_unsafe(TEST_COMMON_CODE_OK),
            TEST_SHORT_CODE_OK
        );
    }

    #[test]
    fn mirror() {
        assert!(is_vertical_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(is_horizontal_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(to_vertical_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(to_horizontal_mirror(TEST_RAW_CODE_ERR).is_err());

        let tmp = is_vertical_mirror(TEST_RAW_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = is_vertical_mirror(TEST_MIRROR_V1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = is_vertical_mirror(TEST_MIRROR_V2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);

        let tmp = to_vertical_mirror(TEST_MIRROR_V1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_V2);
        let tmp = to_vertical_mirror(TEST_MIRROR_V2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_V1);

        let tmp = is_horizontal_mirror(TEST_MIRROR_H);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), true);
        let tmp = is_horizontal_mirror(TEST_MIRROR_H1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = is_horizontal_mirror(TEST_MIRROR_H2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);

        let tmp = to_horizontal_mirror(TEST_MIRROR_H);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H);
        let tmp = to_horizontal_mirror(TEST_MIRROR_H1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H2);
        let tmp = to_horizontal_mirror(TEST_MIRROR_H2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H1);
    }

    #[test]
    fn mirror_unsafe() {
        assert!(is_vertical_mirror_unsafe(TEST_MIRROR_V));
        assert!(!is_vertical_mirror_unsafe(TEST_MIRROR_V1));
        assert!(!is_vertical_mirror_unsafe(TEST_MIRROR_V2));

        assert!(is_horizontal_mirror_unsafe(TEST_MIRROR_H));
        assert!(!is_horizontal_mirror_unsafe(TEST_MIRROR_H1));
        assert!(!is_horizontal_mirror_unsafe(TEST_MIRROR_H2));

        assert_eq!(
            to_vertical_mirror_unsafe(TEST_MIRROR_V),
            TEST_MIRROR_V
        );
        assert_eq!(
            to_vertical_mirror_unsafe(TEST_MIRROR_V1),
            TEST_MIRROR_V2
        );
        assert_eq!(
            to_vertical_mirror_unsafe(TEST_MIRROR_V2),
            TEST_MIRROR_V1
        );

        assert_eq!(
            to_horizontal_mirror_unsafe(TEST_MIRROR_H),
            TEST_MIRROR_H
        );
        assert_eq!(
            to_horizontal_mirror_unsafe(TEST_MIRROR_H1),
            TEST_MIRROR_H2
        );
        assert_eq!(
            to_horizontal_mirror_unsafe(TEST_MIRROR_H2),
            TEST_MIRROR_H1
        );
    }

    #[test]
    fn string() {
        unsafe {
            assert_eq!(Core::SHORT_CODE_STR_SIZE, 6);
            assert_eq!(Core::COMMON_CODE_STR_SIZE, 10);
        }

        // short code string
        assert!(
            short_code_to_string(TEST_SHORT_CODE_ERR).is_err()
        );
        assert!(
            short_code_from_string(TEST_SHORT_CODE_STR_ERR).is_err()
        );

        let tmp = short_code_to_string(TEST_SHORT_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_SHORT_CODE_STR_OK);
        assert_eq!(
            short_code_to_string_unsafe(TEST_SHORT_CODE_OK),
            TEST_SHORT_CODE_STR_OK
        );

        let tmp = short_code_from_string(TEST_SHORT_CODE_STR_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_SHORT_CODE_OK);

        // common code string
        assert!(
            common_code_to_string(TEST_COMMON_CODE_ERR).is_err()
        );
        assert!(
            common_code_to_string_shorten(TEST_COMMON_CODE_ERR).is_err()
        );
        assert!(
            common_code_from_string(TEST_COMMON_CODE_STR_ERR).is_err()
        );

        let tmp = common_code_to_string(TEST_COMMON_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_STR_OK);
        assert_eq!(
            common_code_to_string_unsafe(TEST_COMMON_CODE_OK),
            TEST_COMMON_CODE_STR_OK
        );

        let tmp = common_code_to_string_shorten(TEST_COMMON_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_STR_SHR_OK);
        assert_eq!(
            common_code_to_string_shorten_unsafe(TEST_COMMON_CODE_OK),
            TEST_COMMON_CODE_STR_SHR_OK
        );

        let tmp = common_code_from_string(TEST_COMMON_CODE_STR_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_OK);
        let tmp = common_code_from_string(TEST_COMMON_CODE_STR_SHR_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_OK);
    }
}
