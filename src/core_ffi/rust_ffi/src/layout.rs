use crate::ffi;
use crate::ShortCode;

/// Layout represents a valid klotski situation, which fills single `2x2` block
/// and any number of `1x1` / `1x2` / `2x1` blocks without overlapping within
/// the range of `5x4`, and ensures that at least two spaces need to remain.
/// These layout information will be stored in a 36-bit value (u64), which can
/// uniquely mark a valid klotski situation, and any valid situation can also
/// be represented by a unique value.
#[derive(Debug)]
pub struct Layout {
    value: u64
}

impl Layout {
    /// Check whether the klotski layout value is a valid.
    pub fn check(val: u64) -> bool {
        ffi::layout_check(val)
    }

    /// Create klotski layout without any check. Note that if an invalid value
    /// is passed, unpredictable behavior may occur.
    pub fn unsafe_new(val: u64) -> Self {
        Self { value: val }
    }

    /// Create klotski layout with validity check.
    pub fn new(val: u64) -> Option<Self> {
        if !Self::check(val) {
            return None
        }
        Some(Self::unsafe_new(val))
    }

    /// Create klotski layout from string form.
    pub fn from_str(msg: &str) -> Option<Self> {
        let val = ffi::layout_from_str(msg);
        if val == 0x10_FFFF_FFFF {
            return None;
        }
        Some(Self::unsafe_new(val))
    }
}

impl Layout {
    /// Get the original value of klotski layout.
    pub fn unwrap(&self) -> u64 {
        self.value
    }

    pub fn to_string(&self) -> String {
        ffi::layout_to_str(self.value)
    }

    pub fn to_shorten_string(&self) -> String {
        ffi::layout_to_shorten_str(self.value)
    }

    pub fn to_short_code(&self) -> ShortCode {
        ShortCode::unsafe_new(ffi::layout_to_short_code(self.value))
    }
}

impl Into<u64> for Layout {
    fn into(self) -> u64 {
        self.unwrap()
    }
}

impl Into<String> for Layout {
    fn into(self) -> String {
        self.to_string()
    }
}

impl Into<ShortCode> for Layout {
    fn into(self) -> ShortCode {
        self.to_short_code()
    }
}

impl Layout {
    pub fn is_horizontal_mirror(&self) -> bool {
        ffi::layout_is_horizontal_mirror(self.value)
    }

    pub fn is_vertical_mirror(&self) -> bool {
        ffi::layout_is_vertical_mirror(self.value)
    }

    pub fn to_vertical_mirror(&self) -> Layout {
        let val = ffi::layout_to_vertical_mirror(self.value);
        Layout::unsafe_new(val)
    }

    pub fn to_horizontal_mirror(&self) -> Layout {
        let val = ffi::layout_to_horizontal_mirror(self.value);
        Layout::unsafe_new(val)
    }
}

impl Layout {
    pub fn n_1x1(&self) -> u8 {
        ffi::layout_n_1x1(self.value)
    }

    pub fn n_1x2(&self) -> u8 {
        ffi::layout_n_1x2(self.value)
    }

    pub fn n_2x1(&self) -> u8 {
        ffi::layout_n_2x1(self.value)
    }

    pub fn n_2x2(&self) -> u8 {
        1
    }
}

impl Layout {
    pub fn type_id(&self) -> u8 {
        ffi::layout_type_id(self.value)
    }

    pub fn pattern_id(&self) -> u16 {
        ffi::layout_pattern_id(self.value)
    }

    pub fn toward_char(&self) -> u8 {
        ffi::layout_toward_char(self.value)
    }

    pub fn case_id(&self) -> u32 {
        ffi::layout_case_id(self.value)
    }
}

impl Layout {
    pub fn next_cases(&self) -> Vec<Layout> {
        ffi::layout_next_cases(self.value)
            .iter()
            .map(|val| Layout::unsafe_new(*val))
            .collect()
    }
}

impl PartialEq for Layout {
    fn eq(&self, other: &Self) -> bool {
        self.value == other.value
    }
}

// TODO: add `Display` trait support
