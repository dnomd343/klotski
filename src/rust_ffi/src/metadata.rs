use crate::core::Core;
use Core::CStringDump;

#[derive(Debug)]
pub struct Metadata {
    pub author: String,
    pub system: String,
    pub git_tag: String,
    pub version: String,
    pub compiler: String,
    pub commit_id: String,
    pub build_time: String,
    pub git_branch: String,
    pub project_url: String,
    pub version_major: u32,
    pub version_minor: u32,
    pub version_patch: u32,
}

/// Get klotski core metadata information.
/// # Examples
/// ```
/// use klotski_ffi::metadata;
///
/// println!("{:#?}", metadata());
/// ```
pub fn load_metadata() -> Metadata {
    unsafe {
        Metadata {
            author: Core::get_author().dump(), // const c_char* -> String
            system: Core::get_system_info().dump(),
            git_tag: Core::get_git_tag().dump(),
            version: Core::get_version().dump(),
            compiler: Core::get_compiler_info().dump(),
            commit_id: Core::get_commit_id().dump(),
            build_time: Core::get_build_time().dump(),
            git_branch: Core::get_git_branch().dump(),
            project_url: Core::get_project_url().dump(),
            version_major: Core::get_version_major(), // uint32_t -> u32
            version_minor: Core::get_version_minor(),
            version_patch: Core::get_version_patch(),
        }
    }
}

#[test]
fn tests() {
    let metadata = load_metadata();

    assert_ne!(metadata.author, "");
    assert_ne!(metadata.system, "");
    assert_ne!(metadata.git_tag, "");
    assert_ne!(metadata.version, "");
    assert_ne!(metadata.compiler, "");
    assert_ne!(metadata.commit_id, "");
    assert_ne!(metadata.build_time, "");
    assert_ne!(metadata.git_branch, "");
    assert_ne!(metadata.project_url, "");

    assert!(metadata.version_patch < 65536);
    assert!(metadata.version_minor < 65536);
    assert!(metadata.version_patch < 65536);
}
