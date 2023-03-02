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

pub(crate) fn raw_code_check() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_raw_code_check_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn short_code_check() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_short_code_check_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_check() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_check_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn raw_code_check_random() -> Duration {
    unsafe {
        Duration::from_ns(Core::benchmark_raw_code_check_random_ns())
    }
}

pub(crate) fn short_code_check_random() -> Duration {
    unsafe {
        Duration::from_ns(Core::benchmark_short_code_check_random_ns())
    }
}

pub(crate) fn common_code_check_random() -> Duration {
    unsafe {
        Duration::from_ns(Core::benchmark_common_code_check_random_ns())
    }
}

pub(crate) fn short_code_to_string() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_short_code_to_string_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn short_code_from_string() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_short_code_from_string_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_to_string() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_to_string_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_from_string() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_from_string_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_to_raw_code() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_to_raw_code_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn raw_code_to_common_code() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_raw_code_to_common_code_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_to_short_code() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_to_short_code_us();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_us(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn short_code_to_common_code() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_short_code_to_common_code_us();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_us(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn common_code_to_short_code_fast() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_common_code_to_short_code_fast_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn short_code_to_common_code_fast() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_short_code_to_common_code_fast_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn vertical_mirror_check() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_vertical_mirror_check_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn horizontal_mirror_check() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_horizontal_mirror_check_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn vertical_mirror_convert() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_vertical_mirror_convert_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

pub(crate) fn horizontal_mirror_convert() -> Result<Duration, &'static str> {
    unsafe {
        let time = Core::benchmark_horizontal_mirror_convert_ns();
        match time.total_cmp(&(0 as f64)) {
            Ordering::Greater => Ok(Duration::from_ns(time)),
            _ => Err("data not ready"),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn basic() {
        range_flip();
        preparation();
        warm_up(0x1_000_000);
        assert!(basic_ranges().is_err());
        assert!(all_cases().is_err());
    }

    #[test]
    fn checker() {
        preparation();
        raw_code_check_random();
        short_code_check_random();
        common_code_check_random();
        assert!(raw_code_check().is_ok());
        assert!(short_code_check().is_ok());
        assert!(common_code_check().is_ok());
    }

    #[test]
    fn string() {
        preparation();
        assert!(short_code_to_string().is_ok());
        assert!(short_code_from_string().is_ok());
        assert!(common_code_to_string().is_ok());
        assert!(common_code_from_string().is_ok());
    }

    #[test]
    fn convert() {
        preparation();
        assert!(common_code_to_raw_code().is_ok());
        assert!(raw_code_to_common_code().is_ok());
        assert!(common_code_to_short_code().is_ok());
        assert!(short_code_to_common_code().is_ok());
        assert!(common_code_to_short_code_fast().is_ok());
        assert!(short_code_to_common_code_fast().is_ok());
    }

    #[test]
    fn mirror() {
        preparation();
        assert!(vertical_mirror_check().is_ok());
        assert!(horizontal_mirror_check().is_ok());
        assert!(vertical_mirror_convert().is_ok());
        assert!(horizontal_mirror_convert().is_ok());
    }
}
