use std::fmt;
use super::ffi as codec_ffi;
use super::{ShortCode, CommonCode};

pub struct RawCode {
    code: u64
}

impl Eq for RawCode {}

impl PartialEq for RawCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl fmt::Display for RawCode {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:015X}", self.code)
    }
}

impl RawCode {
    #[inline]
    pub(crate) fn check(raw_code: u64) -> bool {
        codec_ffi::raw_code_check(raw_code)
    }

    #[inline]
    pub(crate) fn new(raw_code: u64) -> RawCode {
        RawCode {
            code: raw_code
        }
    }

    pub fn from(raw_code: u64) -> Result<RawCode, &'static str> {
        match RawCode::check(raw_code) {
            true => Ok(RawCode {
                code: raw_code
            }),
            _ => Err("invalid raw code"),
        }
    }

    // TODO: from ShortCode (u32 / String / CommonCode)
    // TODO: from CommonCode (u64 / String / CommonCode)

    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::raw_code_to_short_code_unsafe(self.code)
        )
    }

    pub fn to_common_code(&self) -> CommonCode {
        CommonCode::new(
            codec_ffi::raw_code_to_common_code_unsafe(self.code)
        )
    }

    pub fn unwrap(&self) -> u64 {
        self.code
    }

    // TODO: mirror function

}
