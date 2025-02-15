use crate::ffi;

#[derive(Debug)]
pub struct ShortCode {
    code: u32
}

impl ShortCode {
    // fn new(code: u32) -> Option<ShortCode> {
    //     if !Self::check(code) {
    //         return None
    //     }
    //     Some(Self::unsafe_new(code))
    // }

    pub fn unsafe_new(code: u32) -> ShortCode {
        Self {code}
    }

    fn check(code: u32) -> bool {
        ffi::short_code_check(code)
    }
}
