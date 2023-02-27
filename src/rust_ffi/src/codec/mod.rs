mod ffi;
mod short_code;
mod common_code;

pub use short_code::ShortCode;
pub use common_code::CommonCode;

pub fn demo() {
    let s = ShortCode::from(4091296).unwrap();
    println!("{}", s.unwrap());
    println!("{:?}", s);
    println!("{}", s);

    let s = ShortCode::from_str("4WVE1").unwrap();
    println!("{}", s);

    println!("ShortCode warm up");
    ShortCode::warm_up();
    println!("ShortCode warm up fast");
    ShortCode::warm_up_fast();

    println!("{}", s.to_string());
    println!("{:?}", s.to_common_code());

    let c = CommonCode::from(0x1A9BF0C00).unwrap();
    println!("{}", c.unwrap());
    println!("{:?}", c);
    println!("{}", c);

    let c = CommonCode::from_str("1A9BF0C00").unwrap();
    println!("{}", c);
    let c = CommonCode::from_str("1A9BF0C").unwrap();
    println!("{}", c);

    println!("{}", c.to_string(false));
    println!("{}", c.to_string(true));
    println!("{}", c.to_short_code());

}
