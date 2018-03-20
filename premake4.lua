#!lua

-- A solution contains projects, and defines the available configurations
solution "MidiUtils"
    objdir "build/obj"
    targetdir "build/bin"
    configurations { "Debug", "Release" }

    -- The main library
    project "MidiUtils"
        kind "StaticLib"
        language "C++"
        includedirs { "include" }
        files { "**.h", "src/**.cpp" }
        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "Release"
            defines { "NDEBUG" }
            flags { "Optimize" }


    -- A Test Example
    project "TestExample"
        kind "ConsoleApp"
        language "C++"
        includedirs { "include" }
        files { "main.cpp" }
        links { "MidiUtils" }
        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "Release"
            defines { "NDEBUG" }
            flags { "Optimize" }