cmake_minimum_required(VERSION 3.31)
project(battleship)

add_executable(battleship src/main.cpp)

# find SDL2 pkg
find_package(SDL2 CONFIG REQUIRED)

#link SDL2
target_link_libraries(battleship PRIVATE SDL2::SDL2 SDL2::SDL2main)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(battleship PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(battleship PUBLIC OpenGL::GL)




