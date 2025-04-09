cmake_minimum_required(VERSION 3.10)
project(chip8)
cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(raylib)
add_executable(chip8 src/main.cpp
                     src/chip8.cpp
                     src/keyboard.cpp
                     src/audio.cpp)
target_link_libraries(chip8 PUBLIC raylib)
