extern crate klotski_ffi;

use klotski_ffi::AllCases;

// use std::thread::sleep;
// use std::time::Duration;

fn main() {
    // println!("{:#?}", klotski_ffi::metadata());

    // println!("{}", klotski_ffi::is_short_code_available());
    // println!("{}", klotski_ffi::is_short_code_available_fast());
    //
    // klotski_ffi::short_code_enable();
    //
    // println!("{}", klotski_ffi::is_short_code_available());
    // println!("{}", klotski_ffi::is_short_code_available_fast());
    //
    // klotski_ffi::short_code_enable_fast();
    //
    // println!("{}", klotski_ffi::is_short_code_available());
    // println!("{}", klotski_ffi::is_short_code_available_fast());

    // let raw_code = klotski_ffi::common_code_to_raw_code(0x1A9BF0C00).unwrap();
    // println!("raw code = {}", raw_code);

    // println!("short code: {}", klotski_ffi::short_code_to_string(12345).unwrap());
    // println!("short code: {}", klotski_ffi::short_code_to_string_unsafe(12345));
    // println!("{}", klotski_ffi::short_code_from_string("12345").unwrap());

    // println!("common code: {}", klotski_ffi::common_code_to_string(0x1A9BF0C00).unwrap());
    // println!("common code: {}", klotski_ffi::common_code_to_string_unsafe(0x1A9BF0C00));
    // println!("common code: {}", klotski_ffi::common_code_to_string_shorten(0x1A9BF0C00).unwrap());
    // println!("common code: {}", klotski_ffi::common_code_to_string_shorten_unsafe(0x1A9BF0C00));

    // println!("{}", klotski_ffi::common_code_from_string("1A9BF0C").unwrap());
    // println!("{}", klotski_ffi::common_code_from_string("1A9BF0C00").unwrap());

    // klotski_ffi::demo();

    // let r = klotski_ffi::RawCode::from(0x0_E58_FC8_5FF_EBC_4DB).unwrap(); // 4FEA13400
    // let r = klotski_ffi::RawCode::from(0x0_EDB_5FF_EBC_5C8_E58).unwrap(); // 8346AFC00
    // let r = klotski_ffi::RawCode::from(0x0_0F9_1CF_FFA_F17_6DA).unwrap(); // 6BFA47000
    // let r = klotski_ffi::RawCode::from(0x0_603_EDF_5CA_FFF_5E2).unwrap(); // 1A9BF0C00

    // println!("{} => {}", r, r.to_common_code());

    // let all_cases = klotski_ffi::all_cases_fetch();
    // for &code in &all_cases[..16] {
    //     println!("{:09X}", code);
    // }

    for raw_code in &AllCases::raw_codes()[..8] {
        println!("{}", raw_code);
    }
    for short_code in &AllCases::short_codes()[..8] {
        println!("{}", short_code);
    }
    for common_code in &AllCases::common_codes()[..8] {
        println!("{}", common_code);
    }

    // loop {
    //     sleep(Duration::from_secs(1));
    // }

}
