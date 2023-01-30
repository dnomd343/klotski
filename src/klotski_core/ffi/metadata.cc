#include "klotski.h"
#include "metadata.h"

uint32_t get_version_major() {
    return VERSION_MAJOR;
}

uint32_t get_version_minor() {
    return VERSION_MINOR;
}

uint32_t get_version_patch() {
    return VERSION_PATCH;
}

const char* get_author() {
    return AUTHOR;
}

const char* get_git_tag() {
    return GIT_TAG_ID;
}

const char* get_version() {
    return VERSION_STR;
}

const char* get_commit_id() {
    return GIT_COMMIT_ID;
}

const char* get_build_time() {
    return BUILD_TIME;
}

const char* get_git_branch() {
    return GIT_BRANCH;
}

const char* get_project_url() {
    return GIT_PROJECT;
}

const char* get_system_info() {
    return SYSTEM;
}

const char* get_compiler_info() {
    return COMPILER;
}
