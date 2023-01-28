#include "klotski.h"
#include "version.h"

int get_version_major() {
    return VERSION_MAJOR;
}

int get_version_minor() {
    return VERSION_MINOR;
}

int get_version_patch() {
    return VERSION_PATCH;
}

const char* get_author() {
    return AUTHOR;
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

const char* get_project_url() {
    return PROJECT_URL;
}

const char* get_system_info() {
    return SYSTEM;
}

const char* get_compiler_info() {
    return COMPILER;
}
