use super::Duration;
use crate::core::Core;
use std::cmp::Ordering;

pub(crate) fn preparation() {
    unsafe {
        Core::benchmark_preparation();
    }
}

pub(crate) fn warm_up(count: u64) -> Duration {
    unsafe {
        Duration::from_us(Core::benchmark_warm_up_us(count))
    }
}

pub(crate) fn range_flip() -> Duration {
    unsafe {
        Duration::from_ns(Core::benchmark_range_flip_ns())
    }
}

pub(crate) fn basic_ranges() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_basic_ranges_ms();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ms(time)),
            _ => Err("data already built"),
        }
    }
}

pub(crate) fn all_cases() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_all_cases_ms();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ms(time)),
            _ => Err("data already built"),
        }
    }
}
