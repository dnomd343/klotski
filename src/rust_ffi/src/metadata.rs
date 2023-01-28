use super::core::Core;
use Core::CStringDump;

#[derive(Debug)]
pub struct Metadata {
    pub author: String,
    pub system: String,
    pub version: String,
    pub compiler: String,
    pub commit_id: String,
    pub build_time: String,
    pub project_url: String,
}

pub fn load_metadata() -> Metadata {
    unsafe {
        Metadata {
            author: Core::get_author().dump(),
            system: Core::get_system_info().dump(),
            version: Core::get_version().dump(),
            compiler: Core::get_compiler_info().dump(),
            commit_id: Core::get_commit_id().dump(),
            build_time: Core::get_build_time().dump(),
            project_url: Core::get_project_url().dump(),
        }
    }
}
