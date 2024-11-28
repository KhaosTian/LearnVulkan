-- root/xmake.lua
set_project("Nova")
set_version("0.1.0")
set_arch("x64")

-- 设置全局配置
set_languages("c++20")
add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

-- 添加包管理
add_requires("vulkansdk", "spdlog", "glfw", "glm", "stb")

-- 引擎静态库
target("runtime")
    -- 基础配置
    set_kind("static")
    
    -- 依赖包
    add_packages("vulkansdk", "spdlog", "glfw", "glm", "stb")

    -- 源文件和头文件
    add_files("source/runtime/**.cpp")
    add_includedirs("source/runtime", {public = true})
    add_headerfiles("source/runtime/**.h", "source/runtime/**.hpp")

target_end()


target("editor")
    -- 基础配置
    set_kind("binary")

    -- 依赖包
    add_packages("vulkansdk", "spdlog", "glfw", "glm", "stb")

    -- 依赖关系
    add_deps("runtime")

    -- 源文件和头文件
    add_files("source/editor/**.cpp")
    add_includedirs("source", {public = true})
    add_headerfiles("source/editor/**.h", "source/editor/**.hpp")
    

target_end()