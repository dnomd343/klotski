use std::fmt;
use super::ffi as codec_ffi;
use super::{ShortCode, CommonCode};

#[derive(Debug)]
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
    /// Output RawCode for debug.
    ///   Format:  XXXXXXXXXXXXXXX (length 15)
    ///   Example: 603EDF5CAFFF5E2
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:015X}", self.code)
    }
}

impl RawCode {
    /// Check if the input `u64` value is a valid raw code.
    #[inline]
    pub(crate) fn check(raw_code: u64) -> bool {
        codec_ffi::raw_code_check(raw_code)
    }

    /// Creates an unchecked raw code, which is not safe and is only used
    /// during encoding conversion (to ensure no errors).
    #[inline]
    pub(crate) fn new(raw_code: u64) -> RawCode {
        RawCode {
            code: raw_code
        }
    }

    /// Create raw code from raw `u64`, and will be checked for validity.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// match RawCode::from(0x0603_EDF5_CAFF_F5E2) {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    /// ```
    pub fn from(raw_code: u64) -> Result<RawCode, &'static str> {
        match RawCode::check(raw_code) {
            true => Ok(RawCode {
                code: raw_code
            }),
            _ => Err("invalid raw code"),
        }
    }
}

impl RawCode {
    // TODO: from ShortCode (u32 / String / CommonCode)
    // TODO: from CommonCode (u64 / String / CommonCode)
}

impl RawCode {
    /// Return the original `u64` type raw code value.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let code = RawCode::from(0x0603_EDF5_CAFF_F5E2).expect("invalid raw code");
    /// println!("original: {}", code.unwrap());
    /// ```
    #[inline]
    pub fn unwrap(&self) -> u64 {
        self.code
    }

    /// Convert RawCode to ShortCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0603_EDF5_CAFF_F5E2).unwrap();
    /// let short_code = raw_code.to_short_code();
    /// println!("{} => {}", raw_code, short_code);
    /// ```
    #[inline]
    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::raw_code_to_short_code_unsafe(self.code)
        )
    }

    /// Convert RawCode to CommonCode type.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0603_EDF5_CAFF_F5E2).unwrap();
    /// let common_code = raw_code.to_common_code();
    /// println!("{} => {}", raw_code, common_code);
    /// ```
    #[inline]
    pub fn to_common_code(&self) -> CommonCode {
        CommonCode::new(
            codec_ffi::raw_code_to_common_code_unsafe(self.code)
        )
    }
}

impl RawCode {

    // TODO: mirror function

}
