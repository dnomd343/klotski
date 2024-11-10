#[cxx::bridge(namespace = "klotski::ffi")]
mod ffi {
    #[derive(Debug)]
    struct RsCommonCode {
        code: u64
    }

    unsafe extern "C++" {
        include!("rust_ffi/include/common_code.h");

        fn to_string(self: &RsCommonCode) -> String;

        fn from_string(s: &str) -> RsCommonCode;
    }
}

pub use ffi::RsCommonCode as CommonCode;

impl CommonCode {
    pub fn unsafe_create(code: u64) -> CommonCode {
        CommonCode { code }
    }

    pub fn unwrap(self: &CommonCode) -> u64 {
        self.code
    }

    pub fn from_string(s: &str) -> CommonCode {
        ffi::from_string(s)
    }
}

impl PartialEq for CommonCode {
    fn eq(&self, other: &Self) -> bool {
        self.code == other.code
    }
}
