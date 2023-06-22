cmake_minimum_required(VERSION 3.0)

project(Velce)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_executable(Velce ${all_SRCS})

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/res/vendor/SDL2-2.26.5/cmake/)
set(SDL2_image_DIR ${CMAKE_SOURCE_DIR}/res/vendor/SDL2_image-2.6.3/cmake/)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_include_directories( Velce
	PUBLIC ${SDL2_INCLUDE_DIRS}
	PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
	PUBLIC Velce/
	PUBLIC res/vendor/imgui/include/
	PUBLIC res/vendor/imgui/src/
)

target_link_libraries(Velce PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image mingw32)
