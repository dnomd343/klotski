#[cxx::bridge(namespace = "klotski::ffi")]
pub(crate) mod ffi {
    #[derive(Debug)]
    struct RsLayout {
        code: u64
    }

    #[derive(Debug)]
    struct RsShortCode {
        code: u32
    }

    unsafe extern "C++" {
        include!("rust_ffi/include/layout.h");

        /// only for internal use
        fn layout_check(val: u64) -> bool;

        /// only for internal use
        fn layout_from_str(s: &str) -> u64;

        fn layout_to_str(val: u64) -> String;

        fn layout_to_shorten_str(val: u64) -> String;

        fn layout_to_short_code(val: u64) -> u32;

        fn layout_is_horizontal_mirror(val: u64) -> bool;

        fn layout_is_vertical_mirror(val: u64) -> bool;

        fn layout_to_horizontal_mirror(val: u64) -> u64;

        fn layout_to_vertical_mirror(val: u64) -> u64;

        fn layout_n_1x1(val: u64) -> u8;

        fn layout_n_1x2(val: u64) -> u8;

        fn layout_n_2x1(val: u64) -> u8;

        fn layout_type_id(val: u64) -> u8;

        fn layout_pattern_id(val: u64) -> u16;

        fn layout_toward_char(val: u64) -> u8;

        fn layout_case_id(val: u64) -> u32;

        fn layout_next_cases(val: u64) -> Vec<u64>;

        /// Convert Layout to string form.
        fn to_string(self: &RsLayout) -> String;

        /// Convert Layout to shorten string form.
        fn to_shorten_string(self: &RsLayout) -> String;

        /// Convert Layout to ShortCode.
        fn to_short_code(self: &RsLayout) -> RsShortCode;

        /// Whether the layout is vertically symmetrical.
        fn is_vertical_mirror(self: &RsLayout) -> bool;

        /// Whether the layout is horizontally symmetrical.
        fn is_horizontal_mirror(self: &RsLayout) -> bool;

        /// Calculate the vertically symmetrical klotski layout.
        fn to_vertical_mirror(self: &RsLayout) -> RsLayout;

        /// Calculate the horizontally symmetrical klotski layout.
        fn to_horizontal_mirror(self: &RsLayout) -> RsLayout;

        /// Obtain number of block 1x1.
        fn n_1x1(self: &RsLayout) -> u8;

        /// Obtain number of block 1x2.
        fn n_1x2(self: &RsLayout) -> u8;

        /// Obtain number of block 2x1.
        fn n_2x1(self: &RsLayout) -> u8;

        /// Obtain number of block 2x2.
        fn n_2x2(self: &RsLayout) -> u8;

        /// Obtain type_id value of current Layout.
        fn type_id(self: &RsLayout) -> u8;

        /// Obtain pattern_id value of current Layout.
        fn pattern_id(self: &RsLayout) -> u16;

        // TODO: add Toward enum for `toward` interface

        // Obtain toward char of current Layout.
        fn toward_char(self: &RsLayout) -> u8;

        /// Obtain case_id value of current Layout.
        fn case_id(self: &RsLayout) -> u32;

        /// Obtain all next cases in Layout.
        fn next_cases(self: &RsLayout) -> Vec<RsLayout>;
    }

    unsafe extern "C++" {
        include!("rust_ffi/include/short_code.h");

        /// only for internal use
        fn short_code_check(val: u32) -> bool;

        /// only for internal use
        fn short_code_from_str(s: &str) -> u32;

        /// Convert ShortCode to string form.
        fn to_string(self: &RsShortCode) -> String;

        /// Convert ShortCode to Layout.
        fn to_layout(self: &RsShortCode) -> RsLayout;

        /// Build the conversion index for ShortCode.
        fn short_code_speed_up(fast_mode: bool);
    }
}

pub use ffi::RsLayout as Layout;
pub use ffi::RsShortCode as ShortCode;

impl Layout {
    /// Check the validity of the original Layout.
    pub fn check(code: u64) -> bool {
        ffi::layout_check(code)
    }

    /// Create Layout without any check.
    pub fn unsafe_create(code: u64) -> Self {
        Self { code }
    }

    /// Create Layout with validity check.
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

    /// Create Layout from string form.
    pub fn from_string(s: &str) -> Option<Self> {
        let val = ffi::layout_from_str(s);
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

impl PartialEq for Layout {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}

impl PartialEq for ShortCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}
