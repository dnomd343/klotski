mod ffi;
mod chore;

use chore::Duration;

pub fn demo() {

    use ffi::*;

    println!("start benchmark\n");
    println!("warm up: {}", warm_up(0x100_0000));

    println!("range flip: {}", range_flip());

    println!("basic ranges: {}", basic_ranges().unwrap());
    println!("all cases: {}", all_cases().unwrap());

    preparation();

    println!("raw code check: {}", raw_code_check().unwrap());
    println!("short code check: {}", short_code_check().unwrap());
    println!("common code check: {}", common_code_check().unwrap());

    println!("raw code check random: {}", raw_code_check_random());
    println!("short code check random: {}", short_code_check_random());
    println!("common code check random: {}", common_code_check_random());

    println!("short code to string: {}", short_code_to_string().unwrap());
    println!("short code from string: {}", short_code_from_string().unwrap());
    println!("common code to string: {}", common_code_to_string().unwrap());
    println!("common code from string: {}", common_code_from_string().unwrap());

    println!("common code to raw code: {}", common_code_to_raw_code().unwrap());
    println!("raw code to common code: {}", raw_code_to_common_code().unwrap());
    println!("common code to short code: {}", common_code_to_short_code().unwrap());
    println!("short code to common code: {}", short_code_to_common_code().unwrap());
    println!("common code to short code fast: {}", common_code_to_short_code_fast().unwrap());
    println!("short code to common code fast: {}", short_code_to_common_code_fast().unwrap());

    println!("vertical mirror check: {}", vertical_mirror_check().unwrap());
    println!("horizontal mirror check: {}", horizontal_mirror_check().unwrap());
    println!("vertical mirror convert: {}", vertical_mirror_convert().unwrap());
    println!("horizontal mirror convert: {}", horizontal_mirror_convert().unwrap());

    println!("\nbenchmark complete");

}
