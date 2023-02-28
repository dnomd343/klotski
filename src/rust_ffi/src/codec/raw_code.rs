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
    /// match RawCode::from(0x0_603_EDF_5CA_FFF_5E2) {
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
    /// Build RawCode from ShortCode.
    /// # Example
    /// ```
    /// use klotski_ffi::{RawCode, ShortCode};
    ///
    /// let r = RawCode::from_short_code(&ShortCode::from(4091296).unwrap());
    ///
    /// println!("result: {}", r);
    /// ```
    #[inline]
    pub fn from_short_code(short_code: &ShortCode) -> RawCode {
        short_code.to_raw_code()
    }

    /// Build RawCode from raw `u32` type short code.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from_short_code_val(4091296).unwrap();
    ///
    /// println!("result: {}", r);
    /// ```
    pub fn from_short_code_val(short_code: u32) -> Result<RawCode, &'static str> {
        let short_code = ShortCode::from(short_code)?;
        Ok(short_code.to_raw_code())
    }

    /// Build RawCode from raw `&str` type short code.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from_short_code_str("4WVE1").unwrap();
    ///
    /// println!("result: {}", r);
    /// ```
    pub fn from_short_code_str(short_code: &str) -> Result<RawCode, &'static str> {
        let short_code = ShortCode::from_str(short_code)?;
        Ok(short_code.to_raw_code())
    }

    /// Build RawCode from CommonCode.
    /// # Example
    /// ```
    /// use klotski_ffi::{RawCode, CommonCode};
    ///
    /// let r = RawCode::from_common_code(&CommonCode::from(0x1_A9BF_0C00).unwrap());
    ///
    /// println!("result: {}", r);
    /// ```
    #[inline]
    pub fn from_common_code(common_code: &CommonCode) -> RawCode {
        common_code.to_raw_code()
    }

    /// Build RawCode from raw `u64` type common code.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from_common_code_val(0x1_A9BF_0C00).unwrap();
    ///
    /// println!("result: {}", r);
    /// ```
    pub fn from_common_code_val(common_code: u64) -> Result<RawCode, &'static str> {
        let common_code = CommonCode::from(common_code)?;
        Ok(common_code.to_raw_code())
    }

    /// Build RawCode from raw `&str` type common code.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from_common_code_str("1A9BF0C00").unwrap();
    ///
    /// println!("result: {}", r);
    /// ```
    pub fn from_common_code_str(common_code: &str) -> Result<RawCode, &'static str> {
        let common_code = CommonCode::from_str(common_code)?;
        Ok(common_code.to_raw_code())
    }
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
    /// Determine whether two layouts are mirrored vertically.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap(); // 8346AFC00
    ///
    /// println!("is vertical mirror: {}", RawCode::is_vertical_mirror(&r1, &r2));
    /// ```
    #[inline]
    pub fn is_vertical_mirror(r1: &RawCode, r2: &RawCode) -> bool {
        r1 == &r2.to_vertical_mirror()
    }

    /// Determine whether two layouts are mirrored horizontally.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap(); // 6BFA47000
    ///
    /// println!("is horizontal mirror: {}", RawCode::is_horizontal_mirror(&r1, &r2));
    /// ```
    #[inline]
    pub fn is_horizontal_mirror(r1: &RawCode, r2: &RawCode) -> bool {
        r1 == &r2.to_horizontal_mirror()
    }

    /// Determine whether the current layout is vertically mirrored with another layout.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap(); // 8346AFC00
    ///
    /// println!("is vertical mirror: {}", r1.is_vertical_mirror_with(&r2));
    /// ```
    #[inline]
    pub fn is_vertical_mirror_with(&self, raw_code: &RawCode) -> bool {
        self == &raw_code.to_vertical_mirror()
    }

    /// Determine whether the current layout is horizontally mirrored with another layout.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap(); // 6BFA47000
    ///
    /// println!("is horizontal mirror: {}", r1.is_horizontal_mirror_with(&r2));
    /// ```
    #[inline]
    pub fn is_horizontal_mirror_with(&self, raw_code: &RawCode) -> bool {
        self == &raw_code.to_horizontal_mirror()
    }

    /// Determine whether the layout is vertically symmetrical.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap(); // 1A9BF0C00
    ///
    /// println!("is vertically symmetrical: {}", r.is_self_vertical_mirror());
    /// ```
    #[inline]
    pub fn is_self_vertical_mirror(&self) -> bool {
        codec_ffi::is_vertical_mirror_unsafe(self.code)
    }

    /// Determine whether the layout is horizontally symmetrical.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap(); // 1A9BF0C00
    ///
    /// println!("is horizontally symmetrical: {}", r.is_self_horizontal_mirror());
    /// ```
    #[inline]
    pub fn is_self_horizontal_mirror(&self) -> bool {
        codec_ffi::is_horizontal_mirror_unsafe(self.code)
    }

    /// Get vertical mirroring of the layout.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// println!("vertical mirror: {}", r.to_vertical_mirror());
    /// ```
    #[inline]
    pub fn to_vertical_mirror(&self) -> RawCode {
        RawCode::new(
            codec_ffi::to_vertical_mirror_unsafe(self.code)
        )
    }

    /// Get horizontally mirroring of the layout.
    /// # Example
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// println!("horizontal mirror: {}", r.to_horizontal_mirror());
    /// ```
    #[inline]
    pub fn to_horizontal_mirror(&self) -> RawCode {
        RawCode::new(
            codec_ffi::to_horizontal_mirror_unsafe(self.code)
        )
    }
}
