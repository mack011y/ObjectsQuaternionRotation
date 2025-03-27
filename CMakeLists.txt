cmake_minimum_required(VERSION 3.20)
project(rotatingFig)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/includes)

set(SOURCES
        src/main.cpp
        src/ModelLoader.cpp
        src/Point3D.cpp
        src/Quaternion.cpp
        src/Render.cpp
)

add_executable(rotatingFig ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(rotatingFig OpenGL::GL GLEW::GLEW glfw)