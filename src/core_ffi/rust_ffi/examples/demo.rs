use klotski::Layout;
use klotski::ShortCode;

fn short_code_demo() {
    ShortCode::speed_up(false);
    ShortCode::speed_up(true);

    assert!(ShortCode::check(4091296));
    let code = ShortCode::create(4091296).unwrap();
    assert_eq!(code, ShortCode::from_string("4WVE1").unwrap());
    assert_eq!(code, ShortCode::create(4091296).unwrap());

    println!("code: {:?}", code.unwrap());
    println!("string: {}", code.to_string());

    println!("layout: {:?}", code.to_layout());
}

fn layout_demo() {
    assert!(Layout::check(0x1A9BF0C00));
    let code = Layout::create(0x1A9BF0C00).unwrap();
    assert_eq!(code, Layout::from_string("1A9BF0C").unwrap());
    assert_eq!(code, Layout::create(0x1A9BF0C00).unwrap());

    println!("code: {:?}", code.unwrap());

    println!("string: {}", code.to_string());
    println!("shorten_string: {}", code.to_shorten_string());

    println!("short_code: {:?}", code.to_short_code());

    println!("is_vertical_mirror: {}", code.is_vertical_mirror());
    println!("is_horizontal_mirror: {}", code.is_horizontal_mirror());

    println!("vertical_mirror: {:?}", code.to_vertical_mirror());
    println!("horizontal_mirror: {:?}", code.to_horizontal_mirror());

    println!("case_info: {}-{}{}-{}", code.type_id(), code.pattern_id(), code.toward_char() as char, code.case_id());

    println!("n_1x1: {}", code.n_1x1());
    println!("n_1x2: {}", code.n_1x2());
    println!("n_2x1: {}", code.n_2x1());
    println!("n_2x2: {}", code.n_2x2());

    println!("next_cases: {:?}", code.next_cases());
}

fn main() {
    println!("----------------------------------------------------------------");
    short_code_demo();
    println!("----------------------------------------------------------------");
    layout_demo();
    println!("----------------------------------------------------------------");
}
