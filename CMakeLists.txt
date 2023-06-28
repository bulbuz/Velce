cmake_minimum_required(VERSION 3.0)

project(Velce)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

FetchContent_Declare(
    crossguid
    GIT_REPOSITORY https://github.com/graeme-hill/crossguid.git
    GIT_TAG master
)

FetchContent_GetProperties(crossguid)
if(NOT crossguid_POPULATED)
    FetchContent_Populate(crossguid)
    add_subdirectory(${crossguid_SOURCE_DIR} ${crossguid_BINARY_DIR})
endif()

add_executable(Velce ${all_SRCS})
target_link_libraries(Velce PRIVATE crossguid)

#set(SDL2_DIR ${CMAKE_SOURCE_DIR}/res/vendor/SDL2-2.26.5/cmake/)
#set(SDL2_image_DIR ${CMAKE_SOURCE_DIR}/res/vendor/SDL2_image-2.6.3/cmake/)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

#set(yaml-cpp_DIR res/vendor/YAML_CPP/lib/cmake/yaml-cpp/)
find_package(yaml-cpp REQUIRED)
target_link_libraries(Velce PRIVATE yaml-cpp)

target_include_directories( Velce
	PUBLIC ${SDL2_INCLUDE_DIRS}
	PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
	PUBLIC Velce/
	PUBLIC res/vendor/imgui/include/
	PUBLIC res/vendor/imgui/src/
)

target_link_libraries(Velce PUBLIC ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
