mod common_code;

use common_code::CommonCode;

fn main() {
    let code = CommonCode::from_string("1A9BF0C");
    println!("code: {:?}", code.unwrap());
    println!("str: {}", code.to_string());

    println!("{}", code == CommonCode::unsafe_create(0x1A9BF0C00));
}
