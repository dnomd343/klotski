use crate::core::Core;

lazy_static! {
    static ref ALL_CASES: Vec<u64> = build_all_cases();
    static ref BASIC_RANGES: Vec<u32> = build_basic_ranges();
}

#[inline]
pub fn all_cases_fetch() -> &'static Vec<u64> {
    &(*ALL_CASES)
}

#[inline]
#[allow(dead_code)]
pub fn basic_ranges_fetch() -> &'static Vec<u32> {
    &(*BASIC_RANGES)
}

fn build_all_cases() -> Vec<u64> {
    unsafe {
        Core::all_cases_build();
        let mut buffer: Vec<u64> = vec![0; Core::ALL_CASES_SIZE as usize];
        Core::export_all_cases(&mut buffer[0]);
        buffer
    }
}

fn build_basic_ranges() -> Vec<u32> {
    unsafe {
        Core::basic_ranges_build();
        let mut buffer: Vec<u32> = vec![0; Core::BASIC_RANGES_SIZE as usize];
        Core::export_basic_ranges(&mut buffer[0]);
        buffer
    }
}

#[cfg(test)]
mod tests {
    use crypto::md5::Md5;
    use crypto::digest::Digest;
    use crate::all_cases::{ALL_CASES, all_cases_fetch, BASIC_RANGES, basic_ranges_fetch};

    const ALL_CASES_MD5: &str = "3888e9fab8d3cbb50908b12b147cfb23";
    const BASIC_RANGES_MD5: &str = "6f385dc171e201089ff96bb010b47212";

    #[test]
    fn all_cases() {
        assert_eq!(&(*ALL_CASES), all_cases_fetch());
    }

    #[test]
    fn basic_ranges() {
        assert_eq!(&(*BASIC_RANGES), basic_ranges_fetch());
    }

    #[test]
    fn all_cases_data() {
        let mut md5 = Md5::new();
        let mut tmp = String::new();
        for &common_code in &(*ALL_CASES) {
            tmp.push_str(&format!("{:09X}\n", common_code));
        }
        md5.input_str(&tmp);
        assert_eq!(md5.result_str(), ALL_CASES_MD5);
    }

    #[test]
    fn basic_ranges_data() {
        let mut md5 = Md5::new();
        let mut tmp = String::new();
        for &range in &(*BASIC_RANGES) {
            tmp.push_str(&format!("{:08X}\n", range));
        }
        md5.input_str(&tmp);
        assert_eq!(md5.result_str(), BASIC_RANGES_MD5);
    }
}
