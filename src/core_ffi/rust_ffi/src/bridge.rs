#[cxx::bridge(namespace = "klotski::ffi")]
mod ffi {
    #[derive(Debug)]
    struct RsCommonCode {
        code: u64,
    }

    #[derive(Debug)]
    struct RsShortCode {
        code: u32,
    }

    unsafe extern "C++" {
        include!("rust_ffi/include/common_code.h");

        /// only for internal use
        fn common_code_check(val: u64) -> bool;

        /// only for internal use
        fn common_code_from_str(s: &str) -> u64;

        /// Convert CommonCode to string form.
        fn to_string(self: &RsCommonCode) -> String;

        /// Convert CommonCode to shorten string form.
        fn to_shorten_string(self: &RsCommonCode) -> String;

        /// Convert CommonCode to ShortCode.
        fn to_short_code(self: &RsCommonCode) -> RsShortCode;

        /// Whether the layout is vertically symmetrical.
        fn is_vertical_mirror(self: &RsCommonCode) -> bool;

        /// Whether the layout is horizontally symmetrical.
        fn is_horizontal_mirror(self: &RsCommonCode) -> bool;

        /// Calculate the vertically symmetrical klotski layout.
        fn to_vertical_mirror(self: &RsCommonCode) -> RsCommonCode;

        /// Calculate the horizontally symmetrical klotski layout.
        fn to_horizontal_mirror(self: &RsCommonCode) -> RsCommonCode;

        /// Obtain all next cases in CommonCode.
        fn next_cases(self: &RsCommonCode) -> Vec<RsCommonCode>;
    }

    unsafe extern "C++" {
        include!("rust_ffi/include/short_code.h");

        /// only for internal use
        fn short_code_check(val: u32) -> bool;

        /// only for internal use
        fn short_code_from_str(s: &str) -> u32;

        /// Convert ShortCode to string form.
        fn to_string(self: &RsShortCode) -> String;

        /// Convert ShortCode to CommonCode.
        fn to_common_code(self: &RsShortCode) -> RsCommonCode;

        /// Build the conversion index for ShortCode.
        fn short_code_speed_up(fast_mode: bool);
    }
}

pub use ffi::RsShortCode as ShortCode;
pub use ffi::RsCommonCode as CommonCode;

impl CommonCode {
    /// Check the validity of the original CommonCode.
    pub fn check(code: u64) -> bool {
        ffi::common_code_check(code)
    }

    /// Create CommonCode without any check.
    pub fn unsafe_create(code: u64) -> Self {
        Self { code }
    }

    /// Create CommonCode with validity check.
    pub fn create(code: u64) -> Option<Self> {
        if Self::check(code) {
            return Some(Self::unsafe_create(code));
        }
        None
    }

    /// Get the original u64 code.
    pub fn unwrap(self: &Self) -> u64 {
        self.code
    }

    /// Create CommonCode from string form.
    pub fn from_string(s: &str) -> Option<Self> {
        let val = ffi::common_code_from_str(s);
        if val < 0x10_FFFF_FFFF {
            return Some(Self::unsafe_create(val));
        }
        None
    }
}

impl ShortCode {
    /// Check the validity of the original ShortCode.
    pub fn check(code: u32) -> bool {
        ffi::short_code_check(code)
    }

    /// Create ShortCode without any check.
    pub fn unsafe_create(code: u32) -> Self {
        Self { code }
    }

    /// Create ShortCode with validity check.
    pub fn create(code: u32) -> Option<Self> {
        if Self::check(code) {
            return Some(Self::unsafe_create(code));
        }
        None
    }

    /// Get the original u32 code.
    pub fn unwrap(self: &Self) -> u32 {
        self.code
    }

    /// Create ShortCode from string form.
    pub fn from_string(s: &str) -> Option<Self> {
        let val = ffi::short_code_from_str(s);
        if val != 0xFFFF_FFFF {
            return Some(Self::unsafe_create(val));
        }
        None
    }

    pub fn speed_up(fast_mode: bool) {
        ffi::short_code_speed_up(fast_mode)
    }
}

impl PartialEq for CommonCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl PartialEq for ShortCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}
