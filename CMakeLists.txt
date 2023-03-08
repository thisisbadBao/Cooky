cmake_minimum_required(VERSION 3.17)
project(cooky)
set (CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wfatal-errors)
set(CMAKE_CXX_FLAGS
	-std=c++17
)
message( STATUS "Before project command: CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}" )
message( STATUS "Before project command: CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}" )
include_directories(libs)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
link_libraries(SDL2)
link_libraries(SDL2_image)
link_libraries(SDL2_ttf)
link_libraries(SDL2_mixer)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_NCLUDE_DIRS})

add_executable(cooky
		src/Game/Game.cpp
		src/Logger/Logger.cpp
		src/ECS/ECS.cpp
		src/AssetStore/AssetStore.cpp
		src/Main.cpp)

target_link_libraries(cooky ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})