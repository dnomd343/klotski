use std::fmt;
use super::ffi as codec_ffi;
use super::{RawCode, ShortCode};

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
    /// Output CommonCode for debug.
    ///   Format:  XXXXXXXXX (length 9)
    ///   Example: 1A9BF0C00
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:09X}", self.code)
    }
}

impl CommonCode {
    /// Check if the input `u64` value is a valid common code.
    #[inline]
    pub(crate) fn check(common_code: u64) -> bool {
        codec_ffi::common_code_check(common_code)
    }

    /// Creates an unchecked common code, which is not safe and is only used
    /// during encoding conversion (to ensure no errors).
    #[inline]
    pub(crate) fn new(common_code: u64) -> CommonCode {
        CommonCode {
            code: common_code
        }
    }

    /// Create common code from raw `u64`, and will be checked for validity.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// match CommonCode::from(0x1_A9BF_0C00) {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    /// ```
    pub fn from(common_code: u64) -> Result<CommonCode, &'static str> {
        match CommonCode::check(common_code) {
            true => Ok(CommonCode {
                code: common_code
            }),
            _ => Err("invalid common code"),
        }
    }

    /// Create common code from raw `&str`, and will be checked for validity.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// match CommonCode::from_str("1A9BF0C00") {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    ///
    /// match CommonCode::from_str("1a9bf0c") {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    /// ```
    pub fn from_str(common_code: &str) -> Result<CommonCode, &'static str> {
        Ok(CommonCode {
            code: codec_ffi::common_code_from_string(common_code)?
        })
    }
}

impl CommonCode {
    // TODO: from RawCode (u64 / RawCode)
    // TODO: from ShortCode (u32 / String / CommonCode)
}

impl CommonCode {
    /// Return the original `u64` type common code value.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let code = CommonCode::from_str("1A9BF0C00").expect("invalid common code");
    /// println!("original: {}", code.unwrap());
    /// ```
    #[inline]
    pub fn unwrap(&self) -> u64 {
        self.code
    }

    /// Returns the common code encoded as a string.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let code = CommonCode::from(0x1_A9BF_0C00).expect("invalid common code");
    /// println!("common code string: {}", code.to_string());
    /// ```
    #[inline]
    pub fn to_string(&self) -> String {
        codec_ffi::common_code_to_string_unsafe(self.code)
    }

    /// Returns the common code encoded as a shorten string, based on the
    /// provisions of the common code, it is allowed to omit the last `0`
    /// of the string (but at least one byte length must be reserved).
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let code = CommonCode::from(0x1_A9BF_0C00).expect("invalid common code");
    /// println!("common code shorten string: {}", code.to_string_shorten());
    /// ```
    #[inline]
    pub fn to_string_shorten(&self) -> String {
        codec_ffi::common_code_to_string_shorten_unsafe(self.code)
    }

    /// Convert CommonCode to RawCode type.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00").unwrap();
    /// let raw_code = common_code.to_raw_code();
    /// println!("{} => {}", common_code, raw_code);
    /// ```
    #[inline]
    pub fn to_raw_code(&self) -> RawCode {
        RawCode::new(
            codec_ffi::common_code_to_raw_code_unsafe(self.code)
        )
    }

    /// Convert CommonCode to ShortCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Example
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00").unwrap();
    /// let short_code = common_code.to_short_code();
    /// println!("{} => {}", common_code, short_code);
    /// ```
    #[inline]
    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::common_code_to_short_code_unsafe(self.code)
        )
    }
}
