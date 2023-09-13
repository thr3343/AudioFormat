cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(AudioFormat LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_executable(AudioFormat)
target_compile_features(AudioFormat PRIVATE cxx_std_23)
target_link_options(AudioFormat PRIVATE -static-libstdc++)
target_compile_options(AudioFormat PRIVATE -march=native -mtune=native -fenable-matrix -save-temps -masm=intel -Ofast -fno-exceptions -Wuninitialized -Weffc++)

	target_sources(AudioFormat
		PRIVATE
		FILE_SET HEADERS
		BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
		FILES
			"src/init.hpp"
			
			
		PRIVATE	
			"src/main.cpp"
	)
			
	