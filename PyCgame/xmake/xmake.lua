add_rules("mode.debug", "mode.release")

add_requires("libsdl2", {configs = {system = false}})
add_requires("libsdl2_image", {configs = {system = false}})
add_requires("libsdl2_mixer", {configs = {system = false}})

target("libjeu")
    set_languages("c23")
    set_kind("shared")
    set_targetdir("../dist")
    add_files("src/**.c")
    add_headerfiles("src/**.h")

    if is_mode("release") then
        -- Ajout du LTO pour optimiser en release
        set_policy("build.optimization.lto", true)
    end

    add_packages("libsdl2", "libsdl2_image", "libsdl2_mixer")

    if is_plat("macosx") then

        add_frameworks("Cocoa", "CoreVideo", "IOKit", "ForceFeedback", "Carbon", "CoreAudio", "AudioToolbox")
        add_defines("MACOS_PLATFORM")
    end