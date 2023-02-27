use std::fmt;
use super::CommonCode;
use super::ffi as codec_ffi;

#[derive(Debug)]
pub struct ShortCode {
    code: u32
}

impl Eq for ShortCode {}

impl PartialEq for ShortCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl fmt::Display for ShortCode {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}({})", self.to_string(), self.code)
    }
}

impl ShortCode {
    pub(crate) fn new(short_code: u32) -> ShortCode {
        ShortCode {
            code: short_code
        }
    }

    pub fn from(short_code: u32) -> Result<ShortCode, &'static str> {
        match codec_ffi::short_code_check(short_code) {
            true => Ok(ShortCode {
                code: short_code
            }),
            _ => Err("invalid short code"),
        }
    }

    pub fn from_str(short_code: &str) -> Result<ShortCode, &'static str> {
        Ok(ShortCode {
            code: codec_ffi::short_code_from_string(short_code)?
        })
    }

    // TODO: from RawCode (u64 / RawCode)
    // TODO: from CommonCode (u64 / String / CommonCode)

    pub fn to_string(&self) -> String {
        codec_ffi::short_code_to_string_unsafe(self.code)
    }

    // TODO: to RawCode

    pub fn to_common_code(&self) -> CommonCode {
        CommonCode::new(
            codec_ffi::short_code_to_common_code_unsafe(self.code)
        )
    }

    pub fn unwrap(&self) -> u32 {
        self.code
    }

    pub fn warm_up() {
        codec_ffi::short_code_enable();
    }

    pub fn warm_up_fast() {
        codec_ffi::short_code_enable_fast();
    }
}
