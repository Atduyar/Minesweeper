---@diagnostic disable: undefined-global, undefined-field
workspace "test" do
	configurations { "Debug", "Release" }
	language "C"
	targetdir "bin/%{cfg.buildcfg}"
	filter "configurations:Debug" do
		warnings "Extra"
	end
end

project "minesweeper" do
	kind "ConsoleApp"
	files { "src/*.c" }
	includedirs { "include", "src" }
	links { "ncurses", "libmine" }

	filter "configurations:Release" do
		optimize "On"
		symbols "Off"
	end

	filter "configurations:Debug" do
		symbols "On"
		optimize "Off"
	end
end

project "libmine" do
	kind "StaticLib"
	files { "src/libmine/src/*.c" }
	includedirs { "include", "src/libmine/include", "src/libmine/src" }

	filter "configurations:Release" do
		optimize "On"
		symbols "Off"
	end

	filter "configurations:Debug" do
		symbols "On"
		optimize "Off"
	end
end
