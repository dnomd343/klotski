extern crate klotski_ffi;

fn main() {
    // println!("{:#?}", klotski_ffi::load_metadata());

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

    println!("short code: {}", klotski_ffi::short_code_to_string(12345).unwrap());

}
