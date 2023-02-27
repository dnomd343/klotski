use std::fmt;
use super::ShortCode;
use super::ffi as codec_ffi;

#[derive(Debug)]
pub struct CommonCode {
    code: u64
}

impl Eq for CommonCode {}

impl PartialEq for CommonCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl fmt::Display for CommonCode {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:09X}", self.code)
    }
}

impl CommonCode {
    pub(crate) fn new(common_code: u64) -> CommonCode {
        CommonCode {
            code: common_code
        }
    }

    pub fn from(common_code: u64) -> Result<CommonCode, &'static str> {
        match codec_ffi::common_code_check(common_code) {
            true => Ok(CommonCode {
                code: common_code
            }),
            _ => Err("invalid common code"),
        }
    }

    pub fn from_str(common_code: &str) -> Result<CommonCode, &'static str> {
        Ok(CommonCode {
            code: codec_ffi::common_code_from_string(common_code)?
        })
    }

    // TODO: from RawCode (u64 / RawCode)
    // TODO: from ShortCode (u32 / String / CommonCode)

    pub fn to_string(&self) -> String {
        codec_ffi::common_code_to_string_unsafe(self.code)
    }

    pub fn to_string_shorten(&self) -> String {
        codec_ffi::common_code_to_string_shorten_unsafe(self.code)
    }

    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::common_code_to_short_code_unsafe(self.code)
        )
    }

    // TODO: to RawCode

    pub fn unwrap(&self) -> u64 {
        self.code
    }
}
