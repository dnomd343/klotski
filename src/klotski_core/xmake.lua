set_xmakever("2.0.0")

set_version("0.1.2")

set_languages("cxx14")

set_optimize("fastest")

set_warnings("everything")

----------------------------------------------------------------

add_includedirs("utils")
add_includedirs("all_cases")

add_includedirs("raw_code")
add_includedirs("short_code")
add_includedirs("common_code")

add_includedirs("core")
add_includedirs("fast_cal")
add_includedirs("analyse")

add_includedirs("benchmark")

----------------------------------------------------------------

target("utils")
    set_kind("static")
    add_files("utils/*.cc")

target("all_cases")
    set_kind("static")
    add_files("all_cases/*.cc")

target("raw_code")
    set_kind("static")
    add_files("raw_code/*.cc")

target("short_code")
    set_kind("static")
    add_includedirs("short_code/offset")
    add_files("short_code/*.cc")

target("common_code")
    set_kind("static")
    add_files("common_code/*.cc")

target("core")
    set_kind("static")
    add_files("core/*.cc")

target("fast_cal")
    set_kind("static")
    add_files("fast_cal/*.cc")

target("analyse")
    set_kind("static")
    add_files("analyse/*.cc")

target("benchmark")
    set_kind("static")
    add_files("benchmark/*.cc")

----------------------------------------------------------------

-- TODO: generate version.h

target("klotski")
    set_kind("static")
    add_includedirs(".")
    add_files("ffi/*.cc")
    add_deps("utils", "all_cases")
    add_deps("raw_code", "short_code", "common_code")
    add_deps("core", "fast_cal", "analyse")
    add_deps("benchmark")

    set_configdir("$(scriptdir)/utils")

    set_configvar("COMPILER_INFO", "GNU ...")
    set_configvar("BUILD_TIME", "...time...")

    add_configfiles("utils/metadata.h.inx", {filename = "config.h"})

    -- add_configfiles("config.h.in", {pattern = "@(.-)@"})

    set_policy("build.merge_archive", true)
--     set_targetdir("../../bin/")

--     os.cp("$(scriptdir)/*.h", "$(scriptdir)/../../bin/")

----------------------------------------------------------------
