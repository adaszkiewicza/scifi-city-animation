cmake_minimum_required(VERSION 3.3)
project(lab4)

set(CMAKE_CXX_STANDARD 11)

set(binary main.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

add_executable(lab4 ${binary}})
target_link_libraries(lab4 ${OPENGL_gl_LIBRARY} glfw)