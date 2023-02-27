mod ffi;
mod short_code;
mod common_code;

use short_code::ShortCode;
use common_code::CommonCode;

pub fn demo() {
    let s = ShortCode::from(12345678).unwrap();
    println!("{:?}", s);
    println!("{}", s);
    println!("{}", s.to_string());

    let s = ShortCode::from_str("CSSBF").unwrap();
    println!("{}", s.unwrap());

    println!("ShortCode warm up");
    ShortCode::warm_up();
    println!("ShortCode warm up fast");
    ShortCode::warm_up_fast();
}
