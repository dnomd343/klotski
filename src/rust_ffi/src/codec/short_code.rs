use std::fmt;
use crate::codec::raw_code::RawCode;
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
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
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
}

impl ShortCode {
    #[inline]
    pub fn unwrap(&self) -> u32 {
        self.code
    }

    #[inline]
    pub fn to_string(&self) -> String {
        codec_ffi::short_code_to_string_unsafe(self.code)
    }

    #[inline]
    pub fn to_raw_code(&self) -> RawCode {
        RawCode::new(
            codec_ffi::short_code_to_raw_code_unsafe(self.code)
        )
    }

    #[inline]
    pub fn to_common_code(&self) -> CommonCode {
        CommonCode::new(
            codec_ffi::short_code_to_common_code_unsafe(self.code)
        )
    }
}

impl ShortCode {
    /// # Introduce
    ///
    /// The conversion of short code must rely on the index. The normal mode will
    /// take a shorter time to build the index, but the conversion efficiency is
    /// low. The fast mode takes more time and memory to build the index and has
    /// a higher conversion efficiency.
    ///
    /// If the conversion is performed without warming up, the warm-up mechanism
    /// will be automatically triggered, which may cause the initial conversion
    /// to take a long time.
    ///
    /// This function will build the index of normal mode, if this has not been
    /// run before, this will block for a certain amount of time, and is safe for
    /// multi-threaded calls.
    ///
    /// If you are already in fast mode, it will have no effect.
    ///
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// ShortCode::warm_up();
    /// ```
    #[inline]
    pub fn warm_up() {
        codec_ffi::short_code_enable();
    }

    /// # Introduce
    ///
    /// This function will build the index of the fast mode, it will consume more
    /// time and memory than normal mode (about 7 times), and it also safe for
    /// multi-threaded calls.
    ///
    /// If you are already in normal mode, it will reduce part of time.
    ///
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// ShortCode::warm_up_fast();
    /// ```
    #[inline]
    pub fn warm_up_fast() {
        codec_ffi::short_code_enable_fast();
    }
}
