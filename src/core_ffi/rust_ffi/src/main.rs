mod common_code;

use common_code::CommonCode;

fn main() {
    assert!(CommonCode::check(0x1A9BF0C00));
    let code = CommonCode::create(0x1A9BF0C00).unwrap();
    assert_eq!(code, CommonCode::from_string("1A9BF0C").unwrap());
    assert_eq!(code, CommonCode::create(0x1A9BF0C00).unwrap());

    println!("code: {:?}", code.unwrap());

    println!("string: {}", code.to_string());
    println!("shorten_string: {}", code.to_shorten_string());

    println!("short_code: {:?}", code.to_short_code());

    println!("is_vertical_mirror: {}", code.is_vertical_mirror());
    println!("is_horizontal_mirror: {}", code.is_horizontal_mirror());

    println!("vertical_mirror: {:?}", code.to_vertical_mirror());
    println!("horizontal_mirror: {:?}", code.to_horizontal_mirror());

    println!("next_cases: {:?}", code.next_cases());
}
