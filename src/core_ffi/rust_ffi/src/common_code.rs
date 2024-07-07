#[cxx::bridge]
pub mod ffi {
    #[derive(Debug)]
    pub struct CommonCode {
        code: u64
    }

    extern "Rust" {}

    unsafe extern "C++" {
        include!("klotski/src/klotski.h");

        // fn unwrap(self: &CommonCode) -> u64;

        // fn from_string(s: &str) -> CommonCode;
        fn from_string() -> CommonCode;
    }
}
