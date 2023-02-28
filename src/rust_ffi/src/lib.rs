#[macro_use]
extern crate lazy_static;

mod core;
mod codec;
mod metadata;
mod all_cases;

pub use codec::RawCode;
pub use codec::ShortCode;
pub use codec::CommonCode;

pub use metadata::load_metadata as metadata;

pub use all_cases::demo;
