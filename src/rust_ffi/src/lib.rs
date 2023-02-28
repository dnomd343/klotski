#[macro_use]
extern crate lazy_static;

mod core;
mod codec;
mod metadata;
mod all_cases;

pub use codec::RawCode;
pub use codec::ShortCode;
pub use codec::CommonCode;

pub use all_cases::all_cases_fetch;

pub use metadata::load_metadata as metadata;
