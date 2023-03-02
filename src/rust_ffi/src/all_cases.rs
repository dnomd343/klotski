use crate::core::Core;
use crate::{RawCode, ShortCode, CommonCode};

#[derive(Debug)]
pub struct AllCases {}

lazy_static! {
    static ref ALL_CASES: Vec<u64> = build_all_cases();
    static ref BASIC_RANGES: Vec<u32> = build_basic_ranges();

    static ref ALL_RAW_CODES: Vec<RawCode> = {
        build_all_cases().iter()
            .map(|code| CommonCode::new(*code).to_raw_code())
            .collect()
    };

    static ref ALL_COMMON_CODES: Vec<CommonCode> = {
        build_all_cases().iter()
            .map(|code| CommonCode::new(*code))
            .collect()
    };

    static ref ALL_SHORT_CODES: Vec<ShortCode> = {
        ShortCode::warm_up_fast();
        build_all_cases().iter()
            .map(|code| CommonCode::new(*code).to_short_code())
            .collect()
    };
}

impl AllCases {
    /// Get all possible layouts and return an vector loaded with raw
    /// `u64` common code.
    #[inline]
    #[allow(dead_code)]
    pub(crate) fn all_cases() -> &'static Vec<u64> {
        &(*ALL_CASES)
    }

    /// Get all possible base ranges with `u32` value.
    #[inline]
    #[allow(dead_code)]
    pub(crate) fn basic_ranges() -> &'static Vec<u32> {
        &(*BASIC_RANGES)
    }

    /// Get all possible layouts and return an vector loaded with RawCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{AllCases, RawCode};
    ///
    /// let all_raw_codes = AllCases::raw_codes();
    ///
    /// assert_eq!(all_raw_codes[..8], vec![
    ///     RawCode::from(0x0_000_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_600_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_E40_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_0C0_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_6C0_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_1C8_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_7C8_000_000_03F_03C).unwrap(),
    ///     RawCode::from(0x0_018_000_000_03F_03C).unwrap(),
    /// ]);
    /// ```
    #[inline]
    pub fn raw_codes() -> &'static Vec<RawCode> {
        &(*ALL_RAW_CODES)
    }

    /// Get all possible layouts and return an vector loaded with ShortCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{AllCases, ShortCode};
    ///
    /// let all_short_codes = AllCases::short_codes();
    ///
    /// assert_eq!(all_short_codes[..8], vec![
    ///     ShortCode::from(0).unwrap(),
    ///     ShortCode::from(1).unwrap(),
    ///     ShortCode::from(2).unwrap(),
    ///     ShortCode::from(3).unwrap(),
    ///     ShortCode::from(4).unwrap(),
    ///     ShortCode::from(5).unwrap(),
    ///     ShortCode::from(6).unwrap(),
    ///     ShortCode::from(7).unwrap(),
    /// ]);
    /// ```
    #[inline]
    pub fn short_codes() -> &'static Vec<ShortCode> {
        &(*ALL_SHORT_CODES)
    }

    /// Get all possible layouts and return an vector loaded with CommonCode.
    /// # Examples
    /// ```
    /// use klotski_ffi::{AllCases, CommonCode};
    ///
    /// let all_common_codes = AllCases::common_codes();
    ///
    /// assert_eq!(all_common_codes[..8], vec![
    ///     CommonCode::from(0x0_0000_0000).unwrap(),
    ///     CommonCode::from(0x0_0000_0003).unwrap(),
    ///     CommonCode::from(0x0_0000_0004).unwrap(),
    ///     CommonCode::from(0x0_0000_000C).unwrap(),
    ///     CommonCode::from(0x0_0000_000F).unwrap(),
    ///     CommonCode::from(0x0_0000_0010).unwrap(),
    ///     CommonCode::from(0x0_0000_001C).unwrap(),
    ///     CommonCode::from(0x0_0000_0030).unwrap(),
    /// ]);
    /// ```
    #[inline]
    pub fn common_codes() -> &'static Vec<CommonCode> {
        &(*ALL_COMMON_CODES)
    }
}

fn build_all_cases() -> Vec<u64> {
    unsafe {
        Core::all_cases_build();
        let mut buffer: Vec<u64> = vec![0; Core::ALL_CASES_SIZE as usize];
        Core::export_all_cases(buffer.as_mut_ptr());
        buffer
    }
}

fn build_basic_ranges() -> Vec<u32> {
    unsafe {
        Core::basic_ranges_build();
        let mut buffer: Vec<u32> = vec![0; Core::BASIC_RANGES_SIZE as usize];
        Core::export_basic_ranges(buffer.as_mut_ptr());
        buffer
    }
}

#[cfg(test)]
mod tests {
    use super::AllCases;
    use crypto::md5::Md5;
    use crypto::digest::Digest;
    use super::{ALL_CASES, BASIC_RANGES};
    use super::{ALL_RAW_CODES, ALL_SHORT_CODES, ALL_COMMON_CODES};

    const ALL_CASES_MD5: &str = "3888e9fab8d3cbb50908b12b147cfb23";
    const BASIC_RANGES_MD5: &str = "6f385dc171e201089ff96bb010b47212";

    #[test]
    fn raw_codes() {
        let tmp: Vec<u64> = ALL_RAW_CODES.iter()
            .map(|code| code.to_common_code().unwrap())
            .collect();
        assert_eq!(tmp, *ALL_CASES);
        assert_eq!(&*ALL_RAW_CODES, AllCases::raw_codes());
    }

    #[test]
    fn short_codes() {
        let tmp: Vec<u64> = ALL_SHORT_CODES.iter()
            .map(|code| code.to_common_code().unwrap())
            .collect();
        assert_eq!(tmp, *ALL_CASES);
        assert_eq!(&*ALL_SHORT_CODES, AllCases::short_codes());
    }

    #[test]
    fn common_codes() {
        let tmp: Vec<u64> = ALL_COMMON_CODES.iter()
            .map(|code| code.unwrap())
            .collect();
        assert_eq!(tmp, *ALL_CASES);
        assert_eq!(&*ALL_COMMON_CODES, AllCases::common_codes());
    }

    #[test]
    fn all_cases_data() {
        let mut md5 = Md5::new();
        let mut tmp = String::new();
        for &common_code in &*ALL_CASES {
            tmp.push_str(&format!("{:09X}\n", common_code));
        }
        md5.input_str(&tmp);
        assert_eq!(md5.result_str(), ALL_CASES_MD5);
        assert_eq!(&*ALL_CASES, AllCases::all_cases());
    }

    #[test]
    fn basic_ranges_data() {
        let mut md5 = Md5::new();
        let mut tmp = String::new();
        for &range in &*BASIC_RANGES {
            tmp.push_str(&format!("{:08X}\n", range));
        }
        md5.input_str(&tmp);
        assert_eq!(md5.result_str(), BASIC_RANGES_MD5);
        assert_eq!(&*BASIC_RANGES, AllCases::basic_ranges());
    }
}
