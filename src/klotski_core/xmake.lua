----------------------------------------------------------------

set_xmakever("2.5.3")

set_project("klotski-core")
set_version("0.1.2", {build = "%Y-%m-%d %H:%M:%S"})
set_languages("cxx14")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
end

if is_mode("release") then
    set_strip("all")
    set_symbols("hidden")
    set_optimize("fastest")
end

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
target_end()

target("all_cases")
    set_kind("object")
    add_files("all_cases/*.cc")
target_end()

target("raw_code")
    set_kind("object")
    add_files("raw_code/*.cc")
target_end()

target("short_code")
    set_kind("object")
    add_includedirs("short_code/offset")
    add_files("short_code/*.cc")
target_end()

target("common_code")
    set_kind("object")
    add_files("common_code/*.cc")
target_end()

target("core")
    set_kind("object")
    add_files("core/*.cc")
target_end()

target("fast_cal")
    set_kind("object")
    add_files("fast_cal/*.cc")
target_end()

target("analyse")
    set_kind("object")
    add_files("analyse/*.cc")
target_end()

target("benchmark")
    set_kind("object")
    add_files("benchmark/*.cc")
target_end()

----------------------------------------------------------------

target("klotski-ffi")
    set_kind("object")
    add_includedirs(".")
    add_files("ffi/*.cc")

    -- TODO: get compiler name and version
    set_configvar("COMPILER", "XMake")

    -- TODO: fix wrong header generate path
    set_configdir("$(scriptdir)/utils")
    add_configfiles("utils/metadata.h.in", {
        filename = "metadata.h", pattern = "@(.-)@"
    })
target_end()

----------------------------------------------------------------

target("klotski")
    set_kind("static")
    add_deps("klotski-ffi")
    add_deps("utils", "all_cases")
    add_deps("raw_code", "short_code", "common_code")
    add_deps("core", "fast_cal", "analyse")
    add_deps("benchmark")
    set_policy("build.merge_archive", true)
    after_build(function (target)
        os.cp(target:targetfile(), "$(projectdir)/bin/")
        os.cp("$(scriptdir)/klotski.h", "$(projectdir)/bin/")
    end)
target_end()

----------------------------------------------------------------
