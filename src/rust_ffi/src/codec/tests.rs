#[cfg(test)]
mod ffi {
    // #[allow(unused_imports)]
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

    const TEST_SHORT_CODE_OK: u32 = 4091296;
    const TEST_SHORT_CODE_STR_OK: &str = "4WVE1";
    const TEST_COMMON_CODE_OK: u64 = 0x1_A9BF_0C00;
    const TEST_COMMON_CODE_STR_OK: &str = "1A9BF0C00";
    const TEST_COMMON_CODE_STR_SHR_OK: &str = "1A9BF0C";
    const TEST_RAW_CODE_OK: u64 = 0x0603_EDF5_CAFF_F5E2;

    const TEST_SHORT_CODE_ERR: u32 = 29670987;
    const TEST_COMMON_CODE_ERR: u64 = 0x1_2190_2300;
    const TEST_RAW_CODE_ERR: u64 = 0x0A34_182B_3810_2D21;

    const TEST_SHORT_CODE_STR_ERR: &str = "R50EH";
    const TEST_COMMON_CODE_STR_ERR: &str = "123J432A9";

    const TEST_MIRROR_V: u64 = 0x0_FC0_480_6DB_FC0_480;
    const TEST_MIRROR_V1: u64 = 0x0_E58_FC8_5FF_EBC_4DB;
    const TEST_MIRROR_V2: u64 = 0x0_EDB_5FF_EBC_5C8_E58;

    const TEST_MIRROR_H: u64 = 0x0_603_EDF_5CA_FFF_5E2;
    const TEST_MIRROR_H1: u64 = 0x0_E58_FC8_5FF_EBC_4DB;
    const TEST_MIRROR_H2: u64 = 0x0_0F9_1CF_FFA_F17_6DA;

    #[test]
    fn checker() {
        assert_eq!(codec_ffi::raw_code_check(TEST_RAW_CODE_OK), true);
        assert_eq!(codec_ffi::raw_code_check(TEST_RAW_CODE_ERR), false);

        assert_eq!(codec_ffi::short_code_check(TEST_SHORT_CODE_OK), true);
        assert_eq!(codec_ffi::short_code_check(TEST_SHORT_CODE_ERR), false);

        assert_eq!(codec_ffi::common_code_check(TEST_COMMON_CODE_OK), true);
        assert_eq!(codec_ffi::common_code_check(TEST_COMMON_CODE_ERR), false);
    }

    #[test]
    fn convert() {
        // raw code -> short code
        assert!(codec_ffi::raw_code_to_short_code(TEST_RAW_CODE_ERR).is_err());
        assert!(codec_ffi::raw_code_to_short_code(TEST_RAW_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::raw_code_to_short_code(TEST_RAW_CODE_OK).unwrap(),
            TEST_SHORT_CODE_OK
        );

        // short code -> raw code
        assert!(codec_ffi::short_code_to_raw_code(TEST_SHORT_CODE_ERR).is_err());
        assert!(codec_ffi::short_code_to_raw_code(TEST_SHORT_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::short_code_to_raw_code(TEST_SHORT_CODE_OK).unwrap(),
            TEST_RAW_CODE_OK
        );

        // raw code -> common code
        assert!(codec_ffi::raw_code_to_common_code(TEST_RAW_CODE_ERR).is_err());
        assert!(codec_ffi::raw_code_to_common_code(TEST_RAW_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::raw_code_to_common_code(TEST_RAW_CODE_OK).unwrap(),
            TEST_COMMON_CODE_OK
        );

        // common code -> raw code
        assert!(codec_ffi::common_code_to_raw_code(TEST_COMMON_CODE_ERR).is_err());
        assert!(codec_ffi::common_code_to_raw_code(TEST_COMMON_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::common_code_to_raw_code(TEST_COMMON_CODE_OK).unwrap(),
            TEST_RAW_CODE_OK
        );

        // short code -> common code
        assert!(codec_ffi::short_code_to_common_code(TEST_SHORT_CODE_ERR).is_err());
        assert!(codec_ffi::short_code_to_common_code(TEST_SHORT_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::short_code_to_common_code(TEST_SHORT_CODE_OK).unwrap(),
            TEST_COMMON_CODE_OK
        );

        // common code -> short code
        assert!(codec_ffi::common_code_to_short_code(TEST_COMMON_CODE_ERR).is_err());
        assert!(codec_ffi::common_code_to_short_code(TEST_COMMON_CODE_OK).is_ok());
        assert_eq!(
            codec_ffi::common_code_to_short_code(TEST_COMMON_CODE_OK).unwrap(),
            TEST_SHORT_CODE_OK
        );
    }

    #[test]
    fn convert_unsafe() {
        // raw code <---> short code
        assert_eq!(
            codec_ffi::raw_code_to_short_code_unsafe(TEST_RAW_CODE_OK),
            TEST_SHORT_CODE_OK
        );
        assert_eq!(
            codec_ffi::short_code_to_raw_code_unsafe(TEST_SHORT_CODE_OK),
            TEST_RAW_CODE_OK
        );

        // raw code <---> common code
        assert_eq!(
            codec_ffi::raw_code_to_common_code_unsafe(TEST_RAW_CODE_OK),
            TEST_COMMON_CODE_OK
        );
        assert_eq!(
            codec_ffi::common_code_to_raw_code_unsafe(TEST_COMMON_CODE_OK),
            TEST_RAW_CODE_OK
        );

        // short code <---> common code
        assert_eq!(
            codec_ffi::short_code_to_common_code_unsafe(TEST_SHORT_CODE_OK),
            TEST_COMMON_CODE_OK
        );
        assert_eq!(
            codec_ffi::common_code_to_short_code_unsafe(TEST_COMMON_CODE_OK),
            TEST_SHORT_CODE_OK
        );
    }

    #[test]
    fn mirror() {
        assert!(codec_ffi::is_vertical_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(codec_ffi::is_horizontal_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(codec_ffi::to_vertical_mirror(TEST_RAW_CODE_ERR).is_err());
        assert!(codec_ffi::to_horizontal_mirror(TEST_RAW_CODE_ERR).is_err());

        let tmp = codec_ffi::is_vertical_mirror(TEST_RAW_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = codec_ffi::is_vertical_mirror(TEST_MIRROR_V1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = codec_ffi::is_vertical_mirror(TEST_MIRROR_V2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);

        let tmp = codec_ffi::to_vertical_mirror(TEST_MIRROR_V1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_V2);
        let tmp = codec_ffi::to_vertical_mirror(TEST_MIRROR_V2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_V1);

        let tmp = codec_ffi::is_horizontal_mirror(TEST_MIRROR_H);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), true);
        let tmp = codec_ffi::is_horizontal_mirror(TEST_MIRROR_H1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);
        let tmp = codec_ffi::is_horizontal_mirror(TEST_MIRROR_H2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), false);

        let tmp = codec_ffi::to_horizontal_mirror(TEST_MIRROR_H);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H);
        let tmp = codec_ffi::to_horizontal_mirror(TEST_MIRROR_H1);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H2);
        let tmp = codec_ffi::to_horizontal_mirror(TEST_MIRROR_H2);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_MIRROR_H1);
    }

    #[test]
    fn mirror_unsafe() {
        assert!(codec_ffi::is_vertical_mirror_unsafe(TEST_MIRROR_V));
        assert!(!codec_ffi::is_vertical_mirror_unsafe(TEST_MIRROR_V1));
        assert!(!codec_ffi::is_vertical_mirror_unsafe(TEST_MIRROR_V2));

        assert!(codec_ffi::is_horizontal_mirror_unsafe(TEST_MIRROR_H));
        assert!(!codec_ffi::is_horizontal_mirror_unsafe(TEST_MIRROR_H1));
        assert!(!codec_ffi::is_horizontal_mirror_unsafe(TEST_MIRROR_H2));

        assert_eq!(
            codec_ffi::to_vertical_mirror_unsafe(TEST_MIRROR_V),
            TEST_MIRROR_V
        );
        assert_eq!(
            codec_ffi::to_vertical_mirror_unsafe(TEST_MIRROR_V1),
            TEST_MIRROR_V2
        );
        assert_eq!(
            codec_ffi::to_vertical_mirror_unsafe(TEST_MIRROR_V2),
            TEST_MIRROR_V1
        );

        assert_eq!(
            codec_ffi::to_horizontal_mirror_unsafe(TEST_MIRROR_H),
            TEST_MIRROR_H
        );
        assert_eq!(
            codec_ffi::to_horizontal_mirror_unsafe(TEST_MIRROR_H1),
            TEST_MIRROR_H2
        );
        assert_eq!(
            codec_ffi::to_horizontal_mirror_unsafe(TEST_MIRROR_H2),
            TEST_MIRROR_H1
        );
    }

    // TODO: string

    // EXPECT_EQ(SHORT_CODE_STR_SIZE, 6);
    // EXPECT_EQ(COMMON_CODE_STR_SIZE, 10);
    //
    // /// short code string verify
    // uint32_t short_code;
    // char short_code_buffer[SHORT_CODE_STR_SIZE];
    // EXPECT_EQ(short_code_to_string(TEST_SHORT_CODE_ERR, short_code_buffer), false);
    // EXPECT_EQ(short_code_from_string(TEST_SHORT_CODE_STR_ERR, &short_code), false);
    //
    // EXPECT_EQ(short_code_to_string(TEST_SHORT_CODE_OK, short_code_buffer), true);
    // EXPECT_STREQ(short_code_buffer, TEST_SHORT_CODE_STR_OK);
    // short_code_to_string_unsafe(TEST_SHORT_CODE_OK, short_code_buffer);
    // EXPECT_STREQ(short_code_buffer, TEST_SHORT_CODE_STR_OK);
    //
    // EXPECT_EQ(short_code_from_string(TEST_SHORT_CODE_STR_OK, &short_code), true);
    // EXPECT_EQ(short_code, TEST_SHORT_CODE_OK);
    //
    // /// common code string verify
    // uint64_t common_code;
    // char common_code_buffer[COMMON_CODE_STR_SIZE];
    // EXPECT_EQ(common_code_to_string(TEST_COMMON_CODE_ERR, common_code_buffer), false);
    // EXPECT_EQ(common_code_from_string(TEST_COMMON_CODE_STR_ERR, &common_code), false);
    //
    // EXPECT_EQ(common_code_to_string(TEST_COMMON_CODE_OK, common_code_buffer), true);
    // EXPECT_STREQ(common_code_buffer, TEST_COMMON_CODE_STR_OK);
    // common_code_to_string_unsafe(TEST_COMMON_CODE_OK, common_code_buffer);
    // EXPECT_STREQ(common_code_buffer, TEST_COMMON_CODE_STR_OK);
    //
    // EXPECT_EQ(common_code_to_string_shorten(TEST_COMMON_CODE_OK, common_code_buffer), true);
    // EXPECT_STREQ(common_code_buffer, TEST_COMMON_CODE_STR_SHR_OK);
    // common_code_to_string_shorten_unsafe(TEST_COMMON_CODE_OK, common_code_buffer);
    // EXPECT_STREQ(common_code_buffer, TEST_COMMON_CODE_STR_SHR_OK);
    //
    // EXPECT_EQ(common_code_from_string(TEST_COMMON_CODE_STR_OK, &common_code), true);
    // EXPECT_EQ(common_code, TEST_COMMON_CODE_OK);
    // EXPECT_EQ(common_code_from_string(TEST_COMMON_CODE_STR_SHR_OK, &common_code), true);
    // EXPECT_EQ(common_code, TEST_COMMON_CODE_OK);

    #[test]
    fn string() {
        // short code string
        assert!(codec_ffi::short_code_to_string(TEST_SHORT_CODE_ERR).is_err());
        assert!(codec_ffi::short_code_from_string(TEST_SHORT_CODE_STR_ERR).is_err());

        let tmp = codec_ffi::short_code_to_string(TEST_SHORT_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_SHORT_CODE_STR_OK);
        assert_eq!(
            codec_ffi::short_code_to_string_unsafe(TEST_SHORT_CODE_OK),
            TEST_SHORT_CODE_STR_OK
        );

        // common code string
        assert!(
            codec_ffi::common_code_to_string(TEST_COMMON_CODE_ERR).is_err()
        );
        assert!(
            codec_ffi::common_code_to_string_shorten(TEST_COMMON_CODE_ERR).is_err()
        );
        assert!(
            codec_ffi::common_code_from_string(TEST_COMMON_CODE_STR_ERR).is_err()
        );

        let tmp = codec_ffi::common_code_to_string(TEST_COMMON_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_STR_OK);
        assert_eq!(
            codec_ffi::common_code_to_string_unsafe(TEST_COMMON_CODE_OK),
            TEST_COMMON_CODE_STR_OK
        );

        let tmp = codec_ffi::common_code_to_string_shorten(TEST_COMMON_CODE_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_STR_SHR_OK);
        assert_eq!(
            codec_ffi::common_code_to_string_shorten_unsafe(TEST_COMMON_CODE_OK),
            TEST_COMMON_CODE_STR_SHR_OK
        );

        let tmp = codec_ffi::common_code_from_string(TEST_COMMON_CODE_STR_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_OK);
        let tmp = codec_ffi::common_code_from_string(TEST_COMMON_CODE_STR_SHR_OK);
        assert!(tmp.is_ok());
        assert_eq!(tmp.unwrap(), TEST_COMMON_CODE_OK);
    }
}
