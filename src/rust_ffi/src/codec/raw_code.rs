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
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0_603_EDF_5CA_FFF_5E2);
    ///
    /// assert!(raw_code.is_ok());
    /// assert_eq!(raw_code.unwrap().unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
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
    /// # Examples
    /// ```
    /// use klotski_ffi::{RawCode, ShortCode};
    ///
    /// let short_code = ShortCode::from(4091296).unwrap();
    /// let raw_code = RawCode::from_short_code(&short_code);
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    #[inline]
    pub fn from_short_code(short_code: &ShortCode) -> RawCode {
        short_code.to_raw_code()
    }

    /// Build RawCode from raw `u32` type short code.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from_short_code_val(4091296).unwrap();
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    pub fn from_short_code_val(short_code: u32) -> Result<RawCode, &'static str> {
        let short_code = ShortCode::from(short_code)?;
        Ok(short_code.to_raw_code())
    }

    /// Build RawCode from raw `&str` type short code.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from_short_code_str("4WVE1").unwrap();
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    pub fn from_short_code_str(short_code: &str) -> Result<RawCode, &'static str> {
        let short_code = ShortCode::from_str(short_code)?;
        Ok(short_code.to_raw_code())
    }

    /// Build RawCode from CommonCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{RawCode, CommonCode};
    ///
    /// let common_code = CommonCode::from(0x1_A9BF_0C00).unwrap();
    /// let raw_code = RawCode::from_common_code(&common_code);
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    #[inline]
    pub fn from_common_code(common_code: &CommonCode) -> RawCode {
        common_code.to_raw_code()
    }

    /// Build RawCode from raw `u64` type common code.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from_common_code_val(0x1_A9BF_0C00).unwrap();
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    pub fn from_common_code_val(common_code: u64) -> Result<RawCode, &'static str> {
        let common_code = CommonCode::from(common_code)?;
        Ok(common_code.to_raw_code())
    }

    /// Build RawCode from raw `&str` type common code.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from_common_code_str("1A9BF0C00").unwrap();
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    pub fn from_common_code_str(common_code: &str) -> Result<RawCode, &'static str> {
        let common_code = CommonCode::from_str(common_code)?;
        Ok(common_code.to_raw_code())
    }
}

impl RawCode {
    /// Return the original `u64` type raw code value.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).expect("invalid raw code");
    ///
    /// assert_eq!(raw_code.unwrap(), 0x0_603_EDF_5CA_FFF_5E2);
    /// ```
    #[inline]
    pub fn unwrap(&self) -> u64 {
        self.code
    }

    /// Convert RawCode to ShortCode type, note that it will take a long time if there
    /// is no warm-up index.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap();
    /// let short_code = raw_code.to_short_code();
    ///
    /// assert_eq!(short_code.unwrap(), 4091296);
    /// ```
    #[inline]
    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::new(
            codec_ffi::raw_code_to_short_code_unsafe(self.code)
        )
    }

    /// Convert RawCode to CommonCode type.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let raw_code = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap();
    /// let common_code = raw_code.to_common_code();
    ///
    /// assert_eq!(common_code.unwrap(), 0x1_A9BF_0C00);
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
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap(); // 8346AFC00
    ///
    /// assert!(RawCode::is_vertical_mirror(&r1, &r2));
    /// ```
    #[inline]
    pub fn is_vertical_mirror(r1: &RawCode, r2: &RawCode) -> bool {
        r1 == &r2.to_vertical_mirror()
    }

    /// Determine whether two layouts are mirrored horizontally.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap(); // 6BFA47000
    ///
    /// assert!(RawCode::is_horizontal_mirror(&r1, &r2));
    /// ```
    #[inline]
    pub fn is_horizontal_mirror(r1: &RawCode, r2: &RawCode) -> bool {
        r1 == &r2.to_horizontal_mirror()
    }

    /// Determine whether the current layout is vertically mirrored with another layout.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap(); // 8346AFC00
    ///
    /// assert!(r1.is_vertical_mirror_with(&r2));
    /// ```
    #[inline]
    pub fn is_vertical_mirror_with(&self, raw_code: &RawCode) -> bool {
        self == &raw_code.to_vertical_mirror()
    }

    /// Determine whether the current layout is horizontally mirrored with another layout.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r1 = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    /// let r2 = RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap(); // 6BFA47000
    ///
    /// assert!(r1.is_horizontal_mirror_with(&r2));
    /// ```
    #[inline]
    pub fn is_horizontal_mirror_with(&self, raw_code: &RawCode) -> bool {
        self == &raw_code.to_horizontal_mirror()
    }

    /// Determine whether the layout is vertically symmetrical.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap(); // 1A9BF0C00
    ///
    /// assert!(!r.is_self_vertical_mirror());
    /// ```
    #[inline]
    pub fn is_self_vertical_mirror(&self) -> bool {
        codec_ffi::is_vertical_mirror_unsafe(self.code)
    }

    /// Determine whether the layout is horizontally symmetrical.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap(); // 1A9BF0C00
    ///
    /// assert!(r.is_self_horizontal_mirror());
    /// ```
    #[inline]
    pub fn is_self_horizontal_mirror(&self) -> bool {
        codec_ffi::is_horizontal_mirror_unsafe(self.code)
    }

    /// Get vertical mirroring of the layout.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    ///
    /// assert_eq!(r.to_vertical_mirror(), RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap());
    /// ```
    #[inline]
    pub fn to_vertical_mirror(&self) -> RawCode {
        RawCode::new(
            codec_ffi::to_vertical_mirror_unsafe(self.code)
        )
    }

    /// Get horizontally mirroring of the layout.
    /// # Examples
    /// ```
    /// use klotski_ffi::RawCode;
    ///
    /// let r = RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    ///
    /// assert_eq!(r.to_horizontal_mirror(), RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap());
    /// ```
    #[inline]
    pub fn to_horizontal_mirror(&self) -> RawCode {
        RawCode::new(
            codec_ffi::to_horizontal_mirror_unsafe(self.code)
        )
    }
}

#[cfg(test)]
pub(crate) mod tests {
    use crate::{RawCode, ShortCode, CommonCode};
    use super::super::short_code::tests as short_code;
    use super::super::common_code::tests as common_code;

    pub(crate) const TEST_CODE: u64 = 0x0_603_EDF_5CA_FFF_5E2;
    pub(crate) const TEST_CODE_ERR: u64 = 0x0_A34_182_B38_102_D21;

    const TEST_MIRROR_V: u64  = 0x0_FC0_480_6DB_FC0_480; // only for test
    const TEST_MIRROR_V1: u64 = 0x0_E58_FC8_5FF_EBC_4DB; // 4FEA13400
    const TEST_MIRROR_V2: u64 = 0x0_EDB_5FF_EBC_5C8_E58; // 8346AFC00

    const TEST_MIRROR_H: u64  = 0x0_603_EDF_5CA_FFF_5E2; // 1A9BF0C00
    const TEST_MIRROR_H1: u64 = 0x0_E58_FC8_5FF_EBC_4DB; // 4FEA13400
    const TEST_MIRROR_H2: u64 = 0x0_0F9_1CF_FFA_F17_6DA; // 6BFA47000

    #[test]
    fn construct() {
        assert!(RawCode::check(TEST_CODE));
        assert!(!RawCode::check(TEST_CODE_ERR));

        assert!(RawCode::from(TEST_CODE).is_ok());
        assert!(RawCode::from(TEST_CODE_ERR).is_err());

        assert_eq!(
            RawCode::new(TEST_CODE),
            RawCode::from(TEST_CODE).unwrap()
        );
    }

    #[test]
    fn convert() {
        // RawCode <-- short code
        assert_eq!(
            RawCode::from_short_code(
                &ShortCode::from(short_code::TEST_CODE).unwrap()
            ),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            RawCode::from_short_code_val(short_code::TEST_CODE).unwrap(),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            RawCode::from_short_code_str(short_code::TEST_CODE_STR).unwrap(),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert!(
            RawCode::from_short_code_val(short_code::TEST_CODE_ERR).is_err()
        );

        assert!(
            RawCode::from_short_code_str(short_code::TEST_CODE_STR_ERR).is_err()
        );

        // RawCode <-- common code
        assert_eq!(
            RawCode::from_common_code(
                &CommonCode::from(common_code::TEST_CODE).unwrap()
            ),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            RawCode::from_common_code_val(common_code::TEST_CODE).unwrap(),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert_eq!(
            RawCode::from_common_code_str(common_code::TEST_CODE_STR).unwrap(),
            RawCode::from(TEST_CODE).unwrap()
        );

        assert!(
            RawCode::from_common_code_val(common_code::TEST_CODE_ERR).is_err()
        );

        assert!(
            RawCode::from_common_code_str(common_code::TEST_CODE_STR_ERR).is_err()
        );
    }

    #[test]
    fn export() {
        assert_eq!(
            RawCode::from(TEST_CODE).unwrap().unwrap(),
            TEST_CODE
        );

        assert_eq!(
            RawCode::from(TEST_CODE).unwrap().to_short_code(),
            ShortCode::from(short_code::TEST_CODE).unwrap()
        );

        assert_eq!(
            RawCode::from(TEST_CODE).unwrap().to_common_code(),
            CommonCode::from(common_code::TEST_CODE).unwrap()
        );
    }

    #[test]
    fn mirror() {
        // Static mirror check

        assert!(RawCode::is_vertical_mirror(
            &RawCode::from(TEST_MIRROR_V1).unwrap(),
            &RawCode::from(TEST_MIRROR_V2).unwrap(),
        ));

        assert!(RawCode::is_horizontal_mirror(
            &RawCode::from(TEST_MIRROR_H1).unwrap(),
            &RawCode::from(TEST_MIRROR_H2).unwrap(),
        ));

        assert!(!RawCode::is_vertical_mirror(
            &RawCode::new(TEST_MIRROR_V),
            &RawCode::from(TEST_MIRROR_V1).unwrap(),
        ));
        assert!(!RawCode::is_vertical_mirror(
            &RawCode::new(TEST_MIRROR_V),
            &RawCode::from(TEST_MIRROR_V2).unwrap(),
        ));

        assert!(!RawCode::is_horizontal_mirror(
            &RawCode::from(TEST_MIRROR_H).unwrap(),
            &RawCode::from(TEST_MIRROR_H1).unwrap(),
        ));
        assert!(!RawCode::is_horizontal_mirror(
            &RawCode::from(TEST_MIRROR_H).unwrap(),
            &RawCode::from(TEST_MIRROR_H2).unwrap(),
        ));

        // Mirror check as member function

        assert!(!RawCode::new(TEST_MIRROR_V).is_vertical_mirror_with(
            &RawCode::from(TEST_MIRROR_V1).unwrap()
        ));
        assert!(!RawCode::new(TEST_MIRROR_V).is_vertical_mirror_with(
            &RawCode::from(TEST_MIRROR_V2).unwrap()
        ));
        assert!(RawCode::new(TEST_MIRROR_V1).is_vertical_mirror_with(
            &RawCode::from(TEST_MIRROR_V2).unwrap()
        ));

        assert!(!RawCode::new(TEST_MIRROR_H).is_horizontal_mirror_with(
            &RawCode::from(TEST_MIRROR_H1).unwrap()
        ));
        assert!(!RawCode::new(TEST_MIRROR_H).is_horizontal_mirror_with(
            &RawCode::from(TEST_MIRROR_H2).unwrap()
        ));
        assert!(RawCode::new(TEST_MIRROR_H1).is_horizontal_mirror_with(
            &RawCode::from(TEST_MIRROR_H2).unwrap()
        ));

        // Symmetry test

        assert!(RawCode::new(TEST_MIRROR_V).is_self_vertical_mirror());
        assert!(!RawCode::new(TEST_MIRROR_V1).is_self_vertical_mirror());
        assert!(!RawCode::new(TEST_MIRROR_V2).is_self_vertical_mirror());

        assert!(RawCode::new(TEST_MIRROR_H).is_self_horizontal_mirror());
        assert!(!RawCode::new(TEST_MIRROR_H1).is_self_horizontal_mirror());
        assert!(!RawCode::new(TEST_MIRROR_H2).is_self_horizontal_mirror());

        // Mirror layout calculations

        assert_eq!(
            RawCode::new(TEST_MIRROR_V).to_vertical_mirror(),
            RawCode::new(TEST_MIRROR_V)
        );
        assert_eq!(
            RawCode::new(TEST_MIRROR_V1).to_vertical_mirror(),
            RawCode::new(TEST_MIRROR_V2)
        );
        assert_eq!(
            RawCode::new(TEST_MIRROR_V2).to_vertical_mirror(),
            RawCode::new(TEST_MIRROR_V1)
        );

        assert_eq!(
            RawCode::new(TEST_MIRROR_H).to_horizontal_mirror(),
            RawCode::new(TEST_MIRROR_H)
        );
        assert_eq!(
            RawCode::new(TEST_MIRROR_H1).to_horizontal_mirror(),
            RawCode::new(TEST_MIRROR_H2)
        );
        assert_eq!(
            RawCode::new(TEST_MIRROR_H2).to_horizontal_mirror(),
            RawCode::new(TEST_MIRROR_H1)
        );
    }
}
