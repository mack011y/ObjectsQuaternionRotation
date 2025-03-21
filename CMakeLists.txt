cmake_minimum_required(VERSION 3.30)
project(rotatingFig)

set(CMAKE_CXX_STANDARD 20)

add_executable(rotatingFig main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(rotatingFig OpenGL::GL GLEW::GLEW glfw)