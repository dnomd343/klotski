----------------------------------------------------------------

set_xmakever("2.5.3")

set_project("klotski-core")
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
    set_kind("object")
    add_files("utils/*.cc")

target("all_cases")
    set_kind("object")
    add_files("all_cases/*.cc")

target("raw_code")
    set_kind("object")
    add_files("raw_code/*.cc")

target("short_code")
    set_kind("object")
    add_includedirs("short_code/offset")
    add_files("short_code/*.cc")

target("common_code")
    set_kind("object")
    add_files("common_code/*.cc")

target("core")
    set_kind("object")
    add_files("core/*.cc")

target("fast_cal")
    set_kind("object")
    add_files("fast_cal/*.cc")

target("analyse")
    set_kind("object")
    add_files("analyse/*.cc")

target("benchmark")
    set_kind("object")
    add_files("benchmark/*.cc")

----------------------------------------------------------------

target("klotski-ffi")
    set_kind("object")
    add_includedirs(".")
    add_files("ffi/*.cc")

    set_configvar("COMPILER", "GCC ...")
    set_configvar("BUILD_TIME", "...time...")

    set_configdir("$(projectdir)/utils")
    add_configfiles("utils/metadata.h.in", {
        filename = "metadata.h", pattern = "@(.-)@"
    })

----------------------------------------------------------------

target("klotski")
    set_kind("static")

    add_deps("klotski-ffi")
    add_deps("utils", "all_cases")
    add_deps("raw_code", "short_code", "common_code")
    add_deps("core", "fast_cal", "analyse")
    add_deps("benchmark")

    set_policy("build.merge_archive", true)
    set_targetdir("$(projectdir)/../../bin/")

    after_build(function (target)
        os.cp("$(projectdir)/klotski.h", "$(projectdir)/../../bin/")
    end)

----------------------------------------------------------------
