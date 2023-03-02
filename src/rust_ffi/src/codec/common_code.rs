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
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from(0x1_A9BF_0C00);
    ///
    /// assert!(common_code.is_ok());
    /// assert_eq!(common_code.unwrap().unwrap(), 0x1_A9BF_0C00);
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
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00");
    ///
    /// assert!(common_code.is_ok());
    /// assert_eq!(common_code.unwrap().unwrap(), 0x1_A9BF_0C00);
    ///
    /// let common_code = CommonCode::from_str("1a9bf0c");
    ///
    /// assert!(common_code.is_ok());
    /// assert_eq!(common_code.unwrap().unwrap(), 0x1_A9BF_0C00);
    /// ```
    pub fn from_str(common_code: &str) -> Result<CommonCode, &'static str> {
        Ok(CommonCode {
            code: codec_ffi::common_code_from_string(common_code)?
        })
    }
}

impl CommonCode {
    /// Build CommonCode from RawCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{RawCode, CommonCode};
    ///
    /// let raw_code = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap();
    /// let common_code = CommonCode::from_raw_code(&raw_code);
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    #[inline]
    pub fn from_raw_code(raw_code: &RawCode) -> CommonCode {
        raw_code.to_common_code()
    }

    /// Build CommonCode from raw `u64` type raw code.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_raw_code_val(0x0_603_EDF_5CA_FFF_5E2).unwrap();
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    pub fn from_raw_code_val(raw_code: u64) -> Result<CommonCode, &'static str> {
        let raw_code = RawCode::from(raw_code)?;
        Ok(raw_code.to_common_code())
    }

    /// Build CommonCode from ShortCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{ShortCode, CommonCode};
    ///
    /// let common_code = CommonCode::from_short_code(&ShortCode::from(4091296).unwrap());
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    #[inline]
    pub fn from_short_code(short_code: &ShortCode) -> CommonCode {
        short_code.to_common_code()
    }

    /// Build CommonCode from raw `u32` type short code.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_short_code_val(4091296).unwrap();
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    pub fn from_short_code_val(short_code: u32) -> Result<CommonCode, &'static str> {
        let short_code = ShortCode::from(short_code)?;
        Ok(short_code.to_common_code())
    }

    /// Build CommonCode from raw `&str` type short code.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_short_code_str("4WVE1").unwrap();
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    pub fn from_short_code_str(short_code: &str) -> Result<CommonCode, &'static str> {
        let short_code = ShortCode::from_str(short_code)?;
        Ok(short_code.to_common_code())
    }
}

impl CommonCode {
    /// Return the original `u64` type common code value.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00").expect("invalid common code");
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
    /// ```
    #[inline]
    pub fn unwrap(&self) -> u64 {
        self.code
    }

    /// Returns the common code encoded as a string.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from(0x1_A9BF_0C00).expect("invalid common code");
    ///
    /// assert_eq!(common_code.to_string(), "1A9BF0C00");
    /// ```
    #[inline]
    pub fn to_string(&self) -> String {
        codec_ffi::common_code_to_string_unsafe(self.code)
    }

    /// Returns the common code encoded as a shorten string, based on the
    /// provisions of the common code, it is allowed to omit the last `0`
    /// of the string (but at least one byte length must be reserved).
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from(0x1_A9BF_0C00).expect("invalid common code");
    ///
    /// assert_eq!(common_code.to_string_shorten(), "1A9BF0C");
    /// ```
    #[inline]
    pub fn to_string_shorten(&self) -> String {
        codec_ffi::common_code_to_string_shorten_unsafe(self.code)
    }

    /// Convert CommonCode to RawCode type.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00").unwrap();
    ///
    /// assert_eq!(common_code.to_raw_code().unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    #[inline]
    pub fn to_raw_code(&self) -> RawCode {
        RawCode::new(
            codec_ffi::common_code_to_raw_code_unsafe(self.code)
        )
    }

    /// Convert CommonCode to ShortCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Examples
    /// ```
    /// use klotski_ffi::CommonCode;
    ///
    /// let common_code = CommonCode::from_str("1A9BF0C00").unwrap();
    ///
    /// assert_eq!(common_code.to_short_code().unwrap(), 4091296);
    /// ```
    #[inline]
    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::common_code_to_short_code_unsafe(self.code)
        )
    }
}

#[cfg(test)]
pub(crate) mod tests {
    use crate::{RawCode, ShortCode, CommonCode};
    use super::super::raw_code::tests as raw_code;
    use super::super::short_code::tests as short_code;

    pub(crate) const TEST_CODE: u64 = 0x1_A9BF_0C00;
    pub(crate) const TEST_CODE_STR: &str = "1A9BF0C00";
    pub(crate) const TEST_CODE_STR_SHR: &str = "1A9BF0C";

    pub(crate) const TEST_CODE_ERR: u64 = 0x1_A9BF_FC00;
    pub(crate) const TEST_CODE_STR_ERR: &str = "123J432A9";

    #[test]
    fn construct() {
        assert!(CommonCode::check(TEST_CODE));
        assert!(!CommonCode::check(TEST_CODE_ERR));

        assert!(CommonCode::from(TEST_CODE).is_ok());
        assert!(CommonCode::from(TEST_CODE_ERR).is_err());

        assert!(CommonCode::from_str(TEST_CODE_STR).is_ok());
        assert!(CommonCode::from_str(TEST_CODE_STR_ERR).is_err());

        assert_eq!(
            CommonCode::new(TEST_CODE),
            CommonCode::from(TEST_CODE).unwrap()
        );
        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap(),
            CommonCode::from_str(TEST_CODE_STR).unwrap()
        );
        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap(),
            CommonCode::from_str(TEST_CODE_STR_SHR).unwrap()
        );
    }

    #[test]
    fn convert() {
        // CommonCode <-- raw code
        assert_eq!(
            CommonCode::from_raw_code(
                &RawCode::from(raw_code::TEST_CODE).unwrap()
            ),
            CommonCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            CommonCode::from_raw_code_val(raw_code::TEST_CODE).unwrap(),
            CommonCode::from(TEST_CODE).unwrap()
        );

        assert!(
            CommonCode::from_raw_code_val(raw_code::TEST_CODE_ERR).is_err()
        );

        // CommonCode <-- short code
        assert_eq!(
            CommonCode::from_short_code(
                &ShortCode::from(short_code::TEST_CODE).unwrap()
            ),
            CommonCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            CommonCode::from_short_code_val(short_code::TEST_CODE).unwrap(),
            CommonCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            CommonCode::from_short_code_str(short_code::TEST_CODE_STR).unwrap(),
            CommonCode::from(TEST_CODE).unwrap()
        );

        assert!(
            CommonCode::from_short_code_val(short_code::TEST_CODE_ERR).is_err()
        );

        assert!(
            CommonCode::from_short_code_str(short_code::TEST_CODE_STR_ERR).is_err()
        );
    }

    #[test]
    fn export() {
        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap().unwrap(),
            TEST_CODE
        );

        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap().to_string(),
            TEST_CODE_STR
        );

        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap().to_string_shorten(),
            TEST_CODE_STR_SHR
        );

        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap().to_raw_code(),
            RawCode::from(raw_code::TEST_CODE).unwrap()
        );

        assert_eq!(
            CommonCode::from(TEST_CODE).unwrap().to_short_code(),
            ShortCode::from(short_code::TEST_CODE).unwrap()
        );
    }
}
