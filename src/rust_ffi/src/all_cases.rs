use crate::core::Core;

lazy_static! {
    static ref BASIC_RANGES: Vec<u32> = {
        unsafe {
            Core::basic_ranges_build();
            let mut buffer: Vec<u32> = vec![0; Core::BASIC_RANGES_SIZE as usize];
            Core::export_basic_ranges(&mut buffer[0]);
            buffer
        }
    };
}

pub fn demo() {

    println!("demo start");



    println!("demo complete");

}

#[cfg(test)]
mod tests {
    use crypto::md5::Md5;
    use crypto::digest::Digest;
    use crate::all_cases::BASIC_RANGES;

    const ALL_CASES_MD5: &str = "3888e9fab8d3cbb50908b12b147cfb23";
    const BASIC_RANGES_MD5: &str = "6f385dc171e201089ff96bb010b47212";

    #[test]
    fn basic_ranges() {
        let mut md5 = Md5::new();
        let mut tmp = String::new();
        for &range in &(*BASIC_RANGES) {
            tmp.push_str(&format!("{:08X}\n", range));
        }
        md5.input_str(&tmp);
        assert_eq!(md5.result_str(), BASIC_RANGES_MD5);
    }
}
