set_project("LearnVulkan")
set_arch("x64")
set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_requires("vulkansdk", "glfw", "glm", "stb")

target("core")
    set_default(true)
    set_kind("static")
    add_includedirs("core")
    add_headerfiles("core/**.h")
    add_headerfiles("core/**.hpp")
    add_files("core/**.cpp")
    add_packages("vulkansdk", "glfw", "glm", "stb")
target_end()

target("editor")
    set_default(true)
    set_kind("binary")
    add_deps("core")
    add_includedirs("core")
    add_includedirs("editor")
    add_headerfiles("editor/**.h")
    add_headerfiles("editor/**.hpp")
    add_files("editor/**.cpp")
    add_packages("vulkansdk", "glfw", "glm", "stb")
target_end()
