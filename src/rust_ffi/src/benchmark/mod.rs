mod ffi;
mod chore;

use chore::Duration;

pub fn demo() {

    use ffi::*;

    // println!("demo: {}", Duration::from_ps(233 as f64).to_string());
    // println!("demo: {}", Duration::from_ns(233 as f64).to_string());
    // println!("demo: {}", Duration::from_us(233 as f64).to_string());
    // println!("demo: {}", Duration::from_ms(233 as f64).to_string());
    // println!("demo: {}", Duration::from_ms(233000 as f64).to_string());

    println!("warm up: {}", warm_up(0x100_0000));

    println!("range flip: {}", range_flip());

    println!("basic ranges: {}", basic_ranges().unwrap());
    println!("all cases: {}", all_cases().unwrap());

    preparation();

    println!("raw code check: {}", raw_code_check().unwrap());
    println!("short code check: {}", short_code_check().unwrap());
    println!("common code check: {}", common_code_check().unwrap());

    println!("raw code check random: {}", raw_code_check_random());
    println!("short code check random: {}", short_code_check_random());
    println!("common code check random: {}", common_code_check_random());

    println!("benchmark complete");

}
