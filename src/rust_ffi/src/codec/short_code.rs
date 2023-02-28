use std::fmt;
use super::ffi as codec_ffi;
use super::{RawCode, CommonCode};

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
    /// Output ShortCode for debug.
    ///   Format:  XXXXX(...num...)
    ///   Example: 4WVE1(4091296)
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}({})", self.to_string(), self.code)
    }
}

impl ShortCode {
    /// Check if the input `u32` value is a valid short code.
    #[inline]
    pub(crate) fn check(short_code: u32) -> bool {
        codec_ffi::short_code_check(short_code)
    }

    /// Creates an unchecked short code, which is not safe and is only used
    /// during encoding conversion (to ensure no errors).
    #[inline]
    pub(crate) fn new(short_code: u32) -> ShortCode {
        ShortCode {
            code: short_code
        }
    }

    /// Create short code from raw `u32`, and will be checked for validity.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// match ShortCode::from(4091296) {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    /// ```
    pub fn from(short_code: u32) -> Result<ShortCode, &'static str> {
        match ShortCode::check(short_code) {
            true => Ok(ShortCode {
                code: short_code
            }),
            _ => Err("invalid short code"),
        }
    }

    /// Create short code from raw `&str`, and will be checked for validity.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// match ShortCode::from_str("4WVE1") {
    ///     Ok(code) => println!("result: {}", code),
    ///     Err(err) => println!("error: {}", err),
    /// }
    /// ```
    pub fn from_str(short_code: &str) -> Result<ShortCode, &'static str> {
        Ok(ShortCode {
            code: codec_ffi::short_code_from_string(short_code)?
        })
    }
}

impl ShortCode {
    /// Build ShortCode from RawCode.
    /// # Example
    /// ```
    /// use klotski_ffi::{RawCode, ShortCode};
    ///
    /// let s = ShortCode::from_raw_code(&RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap());
    ///
    /// println!("result: {}", s);
    /// ```
    #[inline]
    pub fn from_raw_code(raw_code: &RawCode) -> ShortCode {
        raw_code.to_short_code()
    }

    /// Build ShortCode from raw `u64` type raw code.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let s = ShortCode::from_raw_code_val(0x0_603_EDF_5CA_FFF_5E2).unwrap();
    ///
    /// println!("result: {}", s);
    /// ```
    pub fn from_raw_code_val(raw_code: u64) -> Result<ShortCode, &'static str> {
        let raw_code = RawCode::from(raw_code)?;
        Ok(raw_code.to_short_code())
    }

    /// Build ShortCode from CommonCode.
    /// # Example
    /// ```
    /// use klotski_ffi::{ShortCode, CommonCode};
    ///
    /// let s = ShortCode::from_common_code(&CommonCode::from(0x1_A9BF_0C00).unwrap());
    ///
    /// println!("result: {}", s);
    /// ```
    #[inline]
    pub fn from_common_code(common_code: &CommonCode) -> ShortCode {
        common_code.to_short_code()
    }

    /// Build ShortCode from raw `u64` type common code.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let s = ShortCode::from_common_code_val(0x1_A9BF_0C00).unwrap();
    ///
    /// println!("result: {}", s);
    /// ```
    pub fn from_common_code_val(common_code: u64) -> Result<ShortCode, &'static str> {
        let common_code = CommonCode::from(common_code)?;
        Ok(common_code.to_short_code())
    }

    /// Build ShortCode from raw `&str` type common code.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let s = ShortCode::from_common_code_str("1A9BF0C00").unwrap();
    ///
    /// println!("result: {}", s);
    /// ```
    pub fn from_common_code_str(common_code: &str) -> Result<ShortCode, &'static str> {
        let common_code = CommonCode::from_str(common_code)?;
        Ok(common_code.to_short_code())
    }
}

impl ShortCode {
    /// Return the original `u32` type short code value.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let code = ShortCode::from_str("4WVE1").expect("invalid short code");
    /// println!("original: {}", code.unwrap());
    /// ```
    #[inline]
    pub fn unwrap(&self) -> u32 {
        self.code
    }

    /// Returns the short code encoded as a string.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let code = ShortCode::from(4091296).expect("invalid short code");
    /// println!("short code string: {}", code.to_string());
    /// ```
    #[inline]
    pub fn to_string(&self) -> String {
        codec_ffi::short_code_to_string_unsafe(self.code)
    }

    /// Convert ShortCode to RawCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let short_code = ShortCode::from_str("4WVE1").unwrap();
    /// let raw_code = short_code.to_raw_code();
    /// println!("{} => {}", short_code, raw_code);
    /// ```
    #[inline]
    pub fn to_raw_code(&self) -> RawCode {
        RawCode::new(
            codec_ffi::short_code_to_raw_code_unsafe(self.code)
        )
    }

    /// Convert ShortCode to CommonCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Example
    /// ```
    /// use klotski_ffi::ShortCode;
    ///
    /// let short_code = ShortCode::from_str("4WVE1").unwrap();
    /// let common_code = short_code.to_common_code();
    /// println!("{} => {}", short_code, common_code);
    /// ```
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

#[cfg(test)]
mod tests {
    use crate::{RawCode, ShortCode, CommonCode};
    use super::super::raw_code::tests as raw_code;
    use super::super::common_code::tests as common_code;

    const TEST_CODE: u32 = 4091296;
    const TEST_CODE_STR: &str = "4WVE1";

    const TEST_CODE_ERR: u32 = 29670987;
    const TEST_CODE_STR_ERR: &str = "R50EH";

    #[test]
    fn construct() {
        assert!(ShortCode::check(TEST_CODE));
        assert!(!ShortCode::check(TEST_CODE_ERR));

        assert!(ShortCode::from(TEST_CODE).is_ok());
        assert!(ShortCode::from(TEST_CODE_ERR).is_err());

        assert!(ShortCode::from_str(TEST_CODE_STR).is_ok());
        assert!(ShortCode::from_str(TEST_CODE_STR_ERR).is_err());

        assert_eq!(
            ShortCode::new(TEST_CODE),
            ShortCode::from(TEST_CODE).unwrap()
        );
        assert_eq!(
            ShortCode::from(TEST_CODE).unwrap(),
            ShortCode::from_str(TEST_CODE_STR).unwrap()
        );
    }

    #[test]
    fn convert() {
        // ShortCode <-- raw code
        assert_eq!(
            ShortCode::from_raw_code(
                &RawCode::from(raw_code::TEST_CODE).unwrap()
            ),
            ShortCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            ShortCode::from_raw_code_val(raw_code::TEST_CODE).unwrap(),
            ShortCode::from(TEST_CODE).unwrap()
        );

        assert!(
            ShortCode::from_raw_code_val(raw_code::TEST_CODE_ERR).is_err()
        );

        // ShortCode <-- common code
        assert_eq!(
            ShortCode::from_common_code(
                &CommonCode::from(common_code::TEST_CODE).unwrap()
            ),
            ShortCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            ShortCode::from_common_code_val(common_code::TEST_CODE).unwrap(),
            ShortCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            ShortCode::from_common_code_str(common_code::TEST_CODE_STR).unwrap(),
            ShortCode::from(TEST_CODE).unwrap()
        );

        assert!(
            ShortCode::from_common_code_val(common_code::TEST_CODE_ERR).is_err()
        );

        assert!(
            ShortCode::from_common_code_str(common_code::TEST_CODE_STR_ERR).is_err()
        );
    }

    #[test]
    fn export() {
        assert_eq!(ShortCode::from(TEST_CODE).unwrap().unwrap(), TEST_CODE);

        assert_eq!(ShortCode::from(TEST_CODE).unwrap().to_string(), TEST_CODE_STR);

        assert_eq!(
            ShortCode::from(TEST_CODE).unwrap().to_raw_code(),
            RawCode::from(raw_code::TEST_CODE).unwrap()
        );

        assert_eq!(
            ShortCode::from(TEST_CODE).unwrap().to_common_code(),
            CommonCode::from(common_code::TEST_CODE).unwrap()
        );
    }

    #[test]
    fn warm_up() {
        ShortCode::warm_up();
        ShortCode::warm_up_fast();
    }
}
