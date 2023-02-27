use super::ffi as codec_ffi;

#[derive(Debug)]
pub struct CommonCode {
    code: u64
}

impl CommonCode {

    pub(crate) fn new(common_code: u64) -> CommonCode {
        CommonCode {
            code: common_code
        }
    }

}
