-- root/xmake.lua
set_project("nova")
set_version("0.1.0")
set_arch("x64")

-- 设置全局配置
set_languages("c++17")
add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

-- 设置警告级别和编译选项
if is_mode("debug") then
    set_warnings("all", "extra")
    set_optimize("none")
    add_defines("_DEBUG", "NOVA_DEBUG")
elseif is_mode("release") then
    set_warnings("none")
    set_optimize("fastest")
    set_strip("all")
    add_defines("NDEBUG")
end

-- 添加包管理
add_requires("vulkansdk", {configs = {static = true}})
add_requires("glm", {configs = {header_only = true}})
add_requires("spdlog", {configs = {fmt_external = false}})
add_requires("glfw", {configs = {shared = false}})
add_requires("stb", {configs = {shared = false}})

-- 引擎静态库
target("runtime")
    -- 基础配置
    set_kind("static")

    -- 源文件和头文件
    add_files("source/runtime/**.cpp")
    add_includedirs("source/runtime", {public = true})
    add_headerfiles("source/runtime/**.h", "source/runtime/**.hpp")
    
    -- 依赖包
    add_packages("vulkansdk", "spdlog", "glfw", "glm", "stb")
target_end()


target("editor")
    -- 基础配置
    set_kind("binary")

    -- 源文件和头文件
    add_files("source/editor/**.cpp")
    add_includedirs("source", {public = true})
    add_headerfiles("source/editor/**.h", "source/editor/**.hpp")
    
    -- 依赖关系
    add_deps("runtime")
target_end()