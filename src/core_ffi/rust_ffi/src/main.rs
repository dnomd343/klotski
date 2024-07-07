mod common_code;

use common_code::ffi::CommonCode;

fn main() {
    // let code = CommonCode { code: 123 };
    // let val = code.unwrap();
    // println!("val = {}", val);

    let code = common_code::ffi::from_string();
    println!("code = {:?}", code);
}
