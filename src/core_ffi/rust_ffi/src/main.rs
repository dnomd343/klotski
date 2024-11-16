mod bridge;

use bridge::ShortCode;
use bridge::CommonCode;

fn short_code_demo() {
    ShortCode::speed_up(false);
    ShortCode::speed_up(true);

    assert!(ShortCode::check(4091296));
    let code = ShortCode::create(4091296).unwrap();
    assert_eq!(code, ShortCode::from_string("4WVE1").unwrap());
    assert_eq!(code, ShortCode::create(4091296).unwrap());

    println!("code: {:?}", code.unwrap());
    println!("string: {}", code.to_string());

    println!("common_code: {:?}", code.to_common_code());
}

fn common_code_demo() {
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

fn main() {
    println!("----------------------------------------------------------------");
    short_code_demo();
    println!("----------------------------------------------------------------");
    common_code_demo();
    println!("----------------------------------------------------------------");
}
