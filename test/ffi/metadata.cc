#include "klotski.h"
#include "metadata.h"
#include "gtest/gtest.h"

TEST(FFI, metadata) {
    EXPECT_STRNE(get_author(), "");
    EXPECT_STRNE(get_git_tag(), "");
    EXPECT_STRNE(get_version(), "");
    EXPECT_STRNE(get_commit_id(), "");
    EXPECT_STRNE(get_build_time(), "");
    EXPECT_STRNE(get_git_branch(), "");
    EXPECT_STRNE(get_project_url(), "");
    EXPECT_STRNE(get_system_info(), "");
    EXPECT_STRNE(get_compiler_info(), "");

    EXPECT_STREQ(get_author(), AUTHOR);
    EXPECT_STREQ(get_git_tag(), GIT_TAG_ID);
    EXPECT_STREQ(get_version(), VERSION_STR);
    EXPECT_STREQ(get_commit_id(), GIT_COMMIT_ID);
    EXPECT_STREQ(get_build_time(), BUILD_TIME);
    EXPECT_STREQ(get_git_branch(), GIT_BRANCH);
    EXPECT_STREQ(get_project_url(), GIT_PROJECT);
    EXPECT_STREQ(get_system_info(), SYSTEM);
    EXPECT_STREQ(get_compiler_info(), COMPILER);

    EXPECT_EQ(get_version_major(), VERSION_MAJOR);
    EXPECT_EQ(get_version_minor(), VERSION_MINOR);
    EXPECT_EQ(get_version_patch(), VERSION_PATCH);
}
