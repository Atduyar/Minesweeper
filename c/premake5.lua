---@diagnostic disable: undefined-global, undefined-field
workspace "test" do
	configurations { "Release", "Debug" }
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
end

project "minesweeper" do
	kind "ConsoleApp"
	location "build"
	files { "src/*.c" }
	includedirs { "include", "src" }
	links { "ncurses" }

	filter "configurations:Release" do
		optimize "On"
		symbols "Off"
	end

	filter "configurations:Debug" do
		symbols "On"
		optimize "Off"
	end
end
