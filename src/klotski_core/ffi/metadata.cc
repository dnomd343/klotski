#include "klotski.h"
#include "metadata.h"

uint32_t get_version_major() noexcept {
    return VERSION_MAJOR;
}

uint32_t get_version_minor() noexcept {
    return VERSION_MINOR;
}

uint32_t get_version_patch() noexcept {
    return VERSION_PATCH;
}

const char* get_author() noexcept {
    return AUTHOR;
}

const char* get_git_tag() noexcept {
    return GIT_TAG_ID;
}

const char* get_version() noexcept {
    return VERSION_STR;
}

const char* get_commit_id() noexcept {
    return GIT_COMMIT_ID;
}

const char* get_build_time() noexcept {
    return BUILD_TIME;
}

const char* get_git_branch() noexcept {
    return GIT_BRANCH;
}

const char* get_project_url() noexcept {
    return GIT_PROJECT;
}

const char* get_system_info() noexcept {
    return SYSTEM;
}

const char* get_compiler_info() noexcept {
    return COMPILER;
}
