#!lua

-- A solution contains projects, and defines the available configurations
solution "MidiFile"
    configurations { "Debug", "Release" }

    -- A project defines one build target
    project "MidiFile"
        kind "ConsoleApp"
        language "C++"
        files { "**.h", "**.cpp" }

        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "Release"
            defines { "NDEBUG" }
            flags { "Optimize" }