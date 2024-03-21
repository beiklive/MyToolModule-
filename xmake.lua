add_rules("mode.debug", "mode.release")
add_requires("gtest")

target("main")
    set_kind("binary")
    add_includedirs("inc")
    add_files("src/*.cpp")

-- Examples
target("log_main")
    set_kind("binary")
    add_files("example/log_main.cpp")
    add_deps("main")

target("translator_main")
    set_kind("binary")
    add_files("example/translator_main.cpp")
    add_deps("main")

target("json_main")
    set_kind("binary")
    add_files("example/json_main.cpp")
    add_deps("main")



-- Test Cases
target("gtest_json")
    set_kind("binary")
    add_packages("gtest")
    add_files("test/gtest_json.cpp")
    add_deps("main")
