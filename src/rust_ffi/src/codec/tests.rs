mod ffi {
    #[allow(unused_imports)]
    use crate::codec::ffi as codec_ffi;

    #[test]
    fn warm_up() {
        assert_eq!(codec_ffi::is_short_code_available(), false);
        codec_ffi::short_code_enable();
        assert_eq!(codec_ffi::is_short_code_available(), true);

        assert_eq!(codec_ffi::is_short_code_available_fast(), false);
        codec_ffi::short_code_enable_fast();
        assert_eq!(codec_ffi::is_short_code_available_fast(), true);
    }

    #[allow(dead_code)]
    const TEST_SHORT_CODE_OK: u32 = 4091296;

    #[allow(dead_code)]
    const TEST_SHORT_CODE_STR_OK: &str = "4WVE1";

    #[allow(dead_code)]
    const TEST_COMMON_CODE_OK: u64 = 0x1_A9BF_0C00;

    #[allow(dead_code)]
    const TEST_COMMON_CODE_STR_OK: &str = "1A9BF0C00";

    #[allow(dead_code)]
    const TEST_COMMON_CODE_STR_SHR_OK: &str = "1A9BF0C";

    #[allow(dead_code)]
    const TEST_RAW_CODE_OK: u64 = 0x0603_EDF5_CAFF_F5E2;

    #[allow(dead_code)]
    const TEST_SHORT_CODE_ERR: u32 = 29670987;

    #[allow(dead_code)]
    const TEST_COMMON_CODE_ERR: u64 = 0x1_2190_2300;

    #[allow(dead_code)]
    const TEST_RAW_CODE_ERR: u64 = 0x0A34_182B_3810_2D21;

    #[allow(dead_code)]
    const TEST_SHORT_CODE_STR_ERR: &str = "R50EH";

    #[allow(dead_code)]
    const TEST_COMMON_CODE_STR_ERR: &str = "123J432A9";

    #[test]
    fn checker() {
        assert_eq!(codec_ffi::raw_code_check(TEST_RAW_CODE_OK), true);
        assert_eq!(codec_ffi::raw_code_check(TEST_RAW_CODE_ERR), false);

        assert_eq!(codec_ffi::short_code_check(TEST_SHORT_CODE_OK), true);
        assert_eq!(codec_ffi::short_code_check(TEST_SHORT_CODE_ERR), false);

        assert_eq!(codec_ffi::common_code_check(TEST_COMMON_CODE_OK), true);
        assert_eq!(codec_ffi::common_code_check(TEST_COMMON_CODE_ERR), false);
    }

    // TODO: string

    // TODO: convert

    // TODO: convert_unsafe

    // TODO: mirror

    // TODO: mirror_unsafe

}
