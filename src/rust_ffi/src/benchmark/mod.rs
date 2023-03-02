mod ffi;

mod chore;

use chore::Duration;

use ffi::*;

pub fn demo() {

    println!("warm up: {}", warm_up(0x100_0000).to_ms());

    println!("range flip: {}", range_flip().to_ns());

    println!("basic ranges: {}", basic_ranges().unwrap().to_ms());
    println!("all cases: {}", all_cases().unwrap().to_ms());

}
