solution "FruitSeeker"
	configurations {"Debug", "Release"}

	project "FruitSeeker"
		kind "ConsoleApp"

		links {"ThirdParty/LearNN/bin/LearNN"}
		includedirs {"ThirdParty/LearNN/include"}

		links {"ThirdParty/EnvGine/lib/EnvGine"}
		includedirs {"ThirdParty/EnvGine/include"}

		language "C++"
		cppdialect "C++17"
		files {"Source/**.h", "Source/**.cpp"}
		links {"sfml-graphics", "sfml-window", "sfml-system", "pthread"}

		targetdir "bin"

	configuration "Debug"
		defines {"DEBUG"}

	configuration "Release"
		defines {"NDEBUG"}
		
newaction {
	trigger = "clean",
	description = "clean the software",
	execute = function ()
		print("clean the build...")
		os.rmdir("./bin")
		os.rmdir("./obj")
		os.remove("./Makefile")
		os.remove("./*.make")
		print("done.")
	end
}