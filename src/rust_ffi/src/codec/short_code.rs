use std::fmt;
use super::ffi as codec_ffi;

#[derive(Debug)]
pub struct ShortCode {
    code: u32
}

impl ShortCode {
    fn from(short_code: u32) -> Result<ShortCode, &'static str> {
        match codec_ffi::short_code_check(short_code) {
            true => Ok(ShortCode {
                code: short_code
            }),
            _ => Err("invalid short code"),
        }
    }

    fn from_str(short_code: &str) -> Result<ShortCode, &'static str> {
        Ok(ShortCode {
            code: codec_ffi::short_code_from_string(short_code)?
        })
    }

    fn to_string(&self) -> String {
        codec_ffi::short_code_to_string_unsafe(self.code)
    }

    fn unwrap(&self) -> u32 {
        self.code
    }

    fn warm_up() {
        codec_ffi::short_code_enable();
    }

    fn warm_up_fast() {
        codec_ffi::short_code_enable_fast();
    }

    // TODO: to CommonCode
    // TODO: from CommonCode (u32 / String / CommonCode)

}

impl fmt::Display for ShortCode {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // TODO: using ShortCode string output
        write!(f, "{}", self.code)
    }
}

impl PartialEq for ShortCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl Eq for ShortCode {}

pub fn demo() {

    let s1 = ShortCode::from(12345).unwrap();
    let s2 = ShortCode::from(54321).unwrap();

    // println!("ok -> {:?}", s);
    // println!("ok -> {}", s.to_string());

}
