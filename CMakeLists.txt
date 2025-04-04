cmake_minimum_required(VERSION 3.16)
project(viewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем определение для подавления предупреждений об устаревших функциях OpenGL
add_definitions(-DGL_SILENCE_DEPRECATION)

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Собираем список исходных файлов
file(GLOB SOURCES 
    src/*.cpp
    includes/*.h
)

# Создаем исполняемый файл
add_executable(viewer ${SOURCES})

# Подключаем заголовочные файлы
target_include_directories(viewer PRIVATE 
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/includes
)

# Линкуем библиотеки
target_link_libraries(viewer
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)