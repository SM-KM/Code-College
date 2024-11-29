cmake_minimum_required(VERSION 3.30)
project(battleship)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 CONFIG REQUIRED)
find_package(SFML COMPONENTS graphics window system audio REQUIRED)

add_executable(battleship src/main.cpp)

# Link libraries
target_link_libraries(battleship PRIVATE SDL2::SDL2 SDL2::SDL2main)

