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

// struct BasicRange {
//     data: Option<Vec<u32>>
// }
//
// static mut BASIC_RANGE: BasicRange = BasicRange {
//     data: None
// };

// impl BasicRange {
    // fn fetch(&mut self) -> &Vec<u32> {
        // match &self.data {
        //     Some(data) => {
        //         return data;
        //     },
        //     None => (),
        // };

        // &self.data.unwrap()
    // }

    // fn build(&mut self) {
    //     unsafe {
    //         Core::basic_ranges_build();
    //         let mut buffer: Vec<u32> = vec![0; Core::BASIC_RANGES_SIZE as usize];
    //         Core::export_basic_ranges(&mut buffer[0]);
    //         self.data = Some(buffer);
    //     }
    // }
// }

pub fn demo() {

    println!("demo start");

    println!("{:?}", (*BASIC_RANGES).len());

    println!("demo complete");

}
